/**
 * @file
 * @brief Player sound loading
 */
#pragma once

#include <pulsar/player/player.h>
#include <pulsar/bfwav/bfwav.h>
#include <pulsar/bfstm/bfstm.h>

/// Player sound load layout
typedef enum {
	PLSR_PlayerSoundLoadLayout_Channel = 0, ///< One offset per channel where is stored one contiguous chunk of data
	PLSR_PlayerSoundLoadLayout_Blocks, ///< One offset where is stored fixed-size blocks in channel order until the end is reached
} PLSR_PlayerSoundLoadLayout;

/// Player sound load channel layout specific information
typedef struct {
	u32 offsets[PLSR_PLAYER_MAX_CHANNELS]; ///< Offset for each channel where data begins
} PLSR_PlayerSoundLoadChannelLayoutInfo;

/// Player sound load blocks layout specific information
typedef struct {
	u32 firstBlockOffset; ///< Offset where the data of first block of the first channel begins
	u32 blockSize;
	u32 lastBlockPadding;
} PLSR_PlayerSoundLoadBlocksLayoutInfo;

/// Player sound load layout information
typedef struct {
	PLSR_PlayerSoundLoadLayout type;

	union {
		PLSR_PlayerSoundLoadChannelLayoutInfo channel;
		PLSR_PlayerSoundLoadBlocksLayoutInfo blocks;
	};
} PLSR_PlayerSoundLoadLayoutInfo;

typedef struct {
	AudioRendererAdpcmContext context;
	AudioRendererAdpcmParameters parameters;
} PLSR_PlayerSoundAdpcmChannelInfo;

/// Player sound load information
typedef struct {
	const PLSR_Archive* ar; ///< Archive file to read from
	PLSR_PlayerSoundLoadLayoutInfo layout;

	PcmFormat pcmFormat;
	bool looping;
	unsigned int sampleRate;
	unsigned int sampleCount;
	unsigned int loopStartSample; ///< Loop starting sample (if looping is true)
	unsigned int channelCount;
	size_t dataSize; ///< total data size of one channel (if 0 this will be computed according to format and sample count)

	PLSR_PlayerSoundAdpcmChannelInfo adpcm[PLSR_PLAYER_MAX_CHANNELS]; ///< Populated if pcmFormat is PcmFormat_Adpcm
} PLSR_PlayerSoundLoadInfo;

/// Load a sound from a normalized sound load information
PLSR_RC plsrPlayerLoad(const PLSR_PlayerSoundLoadInfo* loadInfo, PLSR_PlayerSoundId* out);
