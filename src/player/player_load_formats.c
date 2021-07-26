#ifdef __SWITCH__

#include <pulsar/player/player_load_formats.h>

#include <pulsar/player/player_load.h>
#include <pulsar/bfwav/bfwav_info.h>
#include <pulsar/bfstm/bfstm_info.h>
#include <pulsar/bfstm/bfstm_channel.h>

#define _LOCAL_TRY(X) PLSR_RC_LTRY(Player, LoadFormats, X)
#define _LOCAL_NX_TRY(X) PLSR_RC_NX_LTRY(Player, LoadFormats, X)
#define _LOCAL_RC_MAKE(X) PLSR_RC_MAKE(Player, LoadFormats, X)

PLSR_RC plsrPlayerLoadWave(const PLSR_BFWAV* bfwav, PLSR_PlayerSoundId* out) {
	PLSR_PlayerSoundLoadInfo loadInfo;
	PLSR_BFWAVInfo waveInfo;

	PLSR_RC_TRY(plsrBFWAVReadInfo(bfwav, &waveInfo));

	switch(waveInfo.format) {
		case PLSR_BFWAVFormat_PCM_8:
			loadInfo.pcmFormat = PcmFormat_Int8;
			break;
		case PLSR_BFWAVFormat_PCM_16:
			loadInfo.pcmFormat = PcmFormat_Int16;
			break;
		case PLSR_BFWAVFormat_DSP_ADPCM:
			loadInfo.pcmFormat = PcmFormat_Adpcm;
			break;
		default:
			return _LOCAL_RC_MAKE(Unsupported);
	}

	loadInfo.layout.type = PLSR_PlayerSoundLoadLayout_Channel;

	loadInfo.ar = &bfwav->ar;
	loadInfo.looping = waveInfo.looping;
	loadInfo.sampleRate = waveInfo.sampleRate;
	loadInfo.sampleCount = waveInfo.sampleCount;
	loadInfo.channelCount = waveInfo.channelInfoTable.info.count > PLSR_PLAYER_MAX_CHANNELS ? PLSR_PLAYER_MAX_CHANNELS : waveInfo.channelInfoTable.info.count;
	loadInfo.dataSize = 0;

	if(waveInfo.looping) {
		loadInfo.loopStartSample = waveInfo.loopStartSample;
	}

	for(u32 channel = 0; channel < loadInfo.channelCount; channel++) {
		PLSR_BFWAVChannelInfo channelInfo;
		_LOCAL_TRY(plsrBFWAVReadChannelInfo(bfwav, &waveInfo.channelInfoTable, channel, &channelInfo));

		loadInfo.layout.channel.offsets[channel] = channelInfo.dataOffset;

		if(waveInfo.format == PLSR_BFWAVFormat_DSP_ADPCM) {
			loadInfo.adpcm[channel].context.index = channelInfo.adpcmInfo.loop.header;
			loadInfo.adpcm[channel].context.history0 = channelInfo.adpcmInfo.loop.yn1;
			loadInfo.adpcm[channel].context.history1 = channelInfo.adpcmInfo.loop.yn2;

			// TODO: static assert libnx params = plsr adpcm coeffs
			memcpy(&loadInfo.adpcm[channel].parameters, &channelInfo.adpcmInfo.coeffs[0], sizeof(AudioRendererAdpcmParameters));
		}
	}

	return plsrPlayerLoad(&loadInfo, out);
}

#endif
