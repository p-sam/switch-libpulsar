#ifdef __SWITCH__

#include <pulsar/player/player_load.h>

#define _LOCAL_TRY(X) PLSR_RC_LTRY(Player, Load, X)
#define _LOCAL_NX_TRY(X) PLSR_RC_NX_LTRY(Player, Load, X)
#define _LOCAL_RC_MAKE(X) PLSR_RC_MAKE(Player, Load, X)
#define _AUDREN_ALIGN(sz) ((sz + (AUDREN_MEMPOOL_ALIGNMENT-1)) &~ (AUDREN_MEMPOOL_ALIGNMENT-1))

static int _getFreeVoiceId(const PLSR_Player* player) {
	for(int id = player->config.startVoiceId; id <= player->config.endVoiceId; id++) {
		if(!player->driver.in_voices[id].is_used) {
			return id;
		}
	}

	return -1;
}

static PLSR_RC _readChannel(const PLSR_PlayerSoundLoadInfo* loadInfo, const PLSR_PlayerSoundLoadChannelLayoutInfo* layoutInfo, size_t dataSize, PLSR_PlayerSound* sound) {
	for(unsigned int channel = 0; channel < sound->channelCount; channel++) {
		_LOCAL_TRY(plsrArchiveReadAt(loadInfo->ar, layoutInfo->offsets[channel], sound->channels[channel].mempool, dataSize));
	}

	return PLSR_RC_OK;
}

static PLSR_RC _readBlocks(const PLSR_PlayerSoundLoadInfo* loadInfo, const PLSR_PlayerSoundLoadBlocksLayoutInfo* layoutInfo, size_t dataSize, PLSR_PlayerSound* sound) {
	u8* channelData[PLSR_PLAYER_MAX_CHANNELS];
	size_t blockCount = dataSize / layoutInfo->blockSize;
	size_t lastBlockSize = dataSize - blockCount * layoutInfo->blockSize;

	for(size_t blockReadCount = 0; blockReadCount < blockCount; blockReadCount++) {
		for(unsigned int channel = 0; channel < loadInfo->channelCount; channel++) {
			if(blockReadCount == 0) {
				channelData[channel] = (u8*)sound->channels[channel].mempool;
			}

			_LOCAL_TRY(plsrArchiveReadAt(
				loadInfo->ar,
				layoutInfo->firstBlockOffset + layoutInfo->blockSize * (blockReadCount * loadInfo->channelCount + channel),
				channelData[channel],
				layoutInfo->blockSize
			));
			channelData[channel] += layoutInfo->blockSize;
		}
	}

	if(lastBlockSize != 0) {
		u32 base = layoutInfo->firstBlockOffset + layoutInfo->blockSize * (blockCount * loadInfo->channelCount);
		for(unsigned int channel = 0; channel < sound->channelCount; channel++) {
			_LOCAL_TRY(plsrArchiveReadAt(
				loadInfo->ar,
				base + ((lastBlockSize + layoutInfo->lastBlockPadding) * channel),
				channelData[channel],
				lastBlockSize
			));
		}
	}

	return PLSR_RC_OK;
}

static PLSR_RC _loadSoundFromInfo(PLSR_Player* player, const PLSR_PlayerSoundLoadInfo* loadInfo, PLSR_PlayerSoundId* out) {
	size_t dataSize = loadInfo->dataSize;

	if(dataSize == 0) {
		switch(loadInfo->pcmFormat) {
			case PcmFormat_Int8:
				dataSize = loadInfo->sampleCount;
				break;
			case PcmFormat_Int16:
				dataSize = loadInfo->sampleCount * 2;
				break;
			case PcmFormat_Adpcm:
				dataSize = loadInfo->sampleCount * 8 + 1;
				dataSize = dataSize / 14 + dataSize % 14;
				break;
			default:
				return _LOCAL_RC_MAKE(Unsupported);
		}
	}

	PLSR_PlayerSound* sound = (PLSR_PlayerSound*)malloc(sizeof(PLSR_PlayerSound));
	if(sound == NULL) {
		return _LOCAL_RC_MAKE(Memory);
	}
	*out = sound;

	bool useSecondLoopWavebuf = loadInfo->looping && loadInfo->loopStartSample != 0;
	size_t alignedDataSize = _AUDREN_ALIGN(dataSize);
	size_t alignedAdpcmParametersSize = loadInfo->pcmFormat == PcmFormat_Adpcm ? _AUDREN_ALIGN(sizeof(AudioRendererAdpcmParameters)) : 0;
	size_t alignedAdpcmContextSize = loadInfo->pcmFormat == PcmFormat_Adpcm ? _AUDREN_ALIGN(sizeof(AudioRendererAdpcmContext)) : 0;
	size_t mempoolSize = alignedDataSize + alignedAdpcmParametersSize + alignedAdpcmContextSize;

	sound->channelCount = 0;
	sound->wavebufCount = useSecondLoopWavebuf ? 2 : 1;
	for(unsigned int channel = 0; channel < loadInfo->channelCount && channel < PLSR_PLAYER_MAX_CHANNELS; channel++) {
		sound->channelCount++;

		memset(&sound->channels[channel].wavebufs[0], 0, sizeof(AudioDriverWaveBuf));
		sound->channels[channel].mempool = NULL;
		sound->channels[channel].mempoolId = -1;
		sound->channels[channel].voiceId = _getFreeVoiceId(player);

		if(sound->channels[channel].voiceId == -1) {
			return _LOCAL_RC_MAKE(Memory);
		}

		if(!audrvVoiceInit(&player->driver, sound->channels[channel].voiceId, 1, loadInfo->pcmFormat, loadInfo->sampleRate)) {
			return _LOCAL_RC_MAKE(System);
		}

		audrvVoiceSetDestinationMix(&player->driver, sound->channels[channel].voiceId, AUDREN_FINAL_MIX_ID);
		for(unsigned int i = 0; i < PLSR_PLAYER_MAX_CHANNELS; i++) {
			audrvVoiceSetMixFactor(
				&player->driver,
				sound->channels[channel].voiceId,
				loadInfo->channelCount == 1 || i == channel ? 0.5f : 0.0f,
				0,
				player->config.sinkChannels[i]
			);
		}

		sound->channels[channel].mempool = memalign(AUDREN_MEMPOOL_ALIGNMENT, mempoolSize);

		if(sound->channels[channel].mempool == NULL) {
			return _LOCAL_RC_MAKE(Memory);
		}

		void* dataPtr = sound->channels[channel].mempool;
		AudioRendererAdpcmParameters* adpcmParameters = (AudioRendererAdpcmParameters*)(dataPtr + alignedDataSize);
		AudioRendererAdpcmContext* adpcmContext = (AudioRendererAdpcmContext*)(dataPtr + alignedDataSize + alignedAdpcmParametersSize);

		sound->channels[channel].wavebufs[0].data_raw = dataPtr;
		sound->channels[channel].wavebufs[0].size = dataSize;
		sound->channels[channel].wavebufs[0].end_sample_offset = loadInfo->sampleCount;
		sound->channels[channel].wavebufs[0].is_looping = loadInfo->looping;

		if(loadInfo->pcmFormat == PcmFormat_Adpcm) {
			memcpy(adpcmContext, &loadInfo->adpcm[channel].context, sizeof(AudioRendererAdpcmContext));
			memcpy(adpcmParameters, &loadInfo->adpcm[channel].parameters, sizeof(AudioRendererAdpcmParameters));

			sound->channels[channel].wavebufs[0].context_addr = adpcmContext;
			sound->channels[channel].wavebufs[0].context_sz = sizeof(AudioRendererAdpcmContext);
			audrvVoiceSetExtraParams(&player->driver, sound->channels[channel].voiceId, adpcmParameters, sizeof(AudioRendererAdpcmParameters));
		}

		sound->channels[channel].mempoolId = audrvMemPoolAdd(&player->driver, sound->channels[channel].mempool, mempoolSize);
		audrvMemPoolAttach(&player->driver, sound->channels[channel].mempoolId);

		if(useSecondLoopWavebuf) {
			memcpy(&sound->channels[channel].wavebufs[1], &sound->channels[channel].wavebufs[0], sizeof(AudioDriverWaveBuf));
			sound->channels[channel].wavebufs[0].end_sample_offset = loadInfo->loopStartSample - 1;
			sound->channels[channel].wavebufs[0].is_looping = false;
			sound->channels[channel].wavebufs[1].start_sample_offset = loadInfo->loopStartSample;
		}
	}

	switch(loadInfo->layout.type) {
		case PLSR_PlayerSoundLoadLayout_Channel:
			_LOCAL_TRY(_readChannel(loadInfo, &loadInfo->layout.channel, dataSize, sound));
			break;
		case PLSR_PlayerSoundLoadLayout_Blocks:
			_LOCAL_TRY(_readBlocks(loadInfo, &loadInfo->layout.blocks, dataSize, sound));
			break;
		default:
			return _LOCAL_RC_MAKE(Unsupported);
	}

	for(unsigned int channel = 0; channel < sound->channelCount; channel++) {
		armDCacheFlush(sound->channels[channel].mempool, mempoolSize);
	}

	audrvUpdate(&player->driver);

	return PLSR_RC_OK;
}

PLSR_RC plsrPlayerLoad(const PLSR_PlayerSoundLoadInfo* loadInfo, PLSR_PlayerSoundId* out) {
	PLSR_Player* player = plsrPlayerGetInstance();
	if(player == NULL) {
		return _LOCAL_RC_MAKE(NotReady);
	}

	PLSR_PlayerSoundId id = NULL;
	PLSR_RC rc = _loadSoundFromInfo(player, loadInfo, &id);

	if(PLSR_RC_SUCCEEDED(rc)) {
		*out = id;
	} else {
		*out = NULL;
		plsrPlayerFree(id);
	}

	return rc;
}

#endif
