#pragma once

// @see https://github.com/Kinnay/Nintendo-File-Formats/wiki/BFSTM-File-Format
// @see http://mk8.tockdom.com/wiki/BFSTM_(File_Format)

#include <pulsar/types.h>
#include <pulsar/archive/archive_internal.h>

#define _PLSR_BFSTM_MAGIC "FSTM"
#define _PLSR_BFSTM_INFO_MAGIC "INFO"
#define _PLSR_BFSTM_DATA_MAGIC "DATA"

#define _PLSR_BFSTM_SECTION_IDENTIFIER_INFO 0x4000
#define _PLSR_BFSTM_SECTION_IDENTIFIER_DATA 0x4002

#define _PLSR_BFSTM_INFO_IDENTIFIER_STREAM_INFO 0x4100
#define _PLSR_BFSTM_INFO_IDENTIFIER_TRACK_TABLE 0x0101
#define _PLSR_BFSTM_INFO_IDENTIFIER_CHANNEL_TABLE 0x0101

#define _PLSR_BFSTM_INFO_IDENTIFIER_STREAM_DATA 0x1F00
#define _PLSR_BFSTM_INFO_IDENTIFIER_CHANNEL_ENTRY 0x4102
#define _PLSR_BFSTM_INFO_IDENTIFIER_ADPCM_ENTRY 0x0300

#define _PLSR_BFSTM_FORMAT_PCM_8 0
#define _PLSR_BFSTM_FORMAT_PCM_16 1
#define _PLSR_BFSTM_FORMAT_DSP_ADPCM 2

typedef struct {
	u8 format;
	u8 looping;
	char _unkX03[2];
	u32 sampleRate;
	u32 loopStartSample;
	u32 sampleCount;
	u32 blockCount;
	u32 blockSize;
	u32 blockSampleCount;
	u32 lastBlockSize;
	u32 lastBlockSampleCount;
	u32 lastBlockSizeWithPadding;
	u32 seekInfoSize;
	u32 seekSampleInterval;
	_PLSR_ArchiveRef dataRef;
} _PLSR_BFSTMInfo;

typedef struct {
	_PLSR_ArchiveRef adpcmInfoRef;
} _PLSR_BFSTMChannelInfo;
