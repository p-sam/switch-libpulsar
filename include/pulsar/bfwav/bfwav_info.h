#pragma once

#include <pulsar/bfwav/bfwav.h>

typedef struct {
	u16 header;
	s16 yn1;
	s16 yn2;
} PLSR_BFWAVAdpcmContextInfo;

typedef struct {
	u16 coeffs[16];
	PLSR_BFWAVAdpcmContextInfo main;
	PLSR_BFWAVAdpcmContextInfo loop;
} PLSR_BFWAVAdpcmInfo;

typedef struct {
	u32 dataOffset;
	PLSR_BFWAVAdpcmInfo adpcmInfo;
} PLSR_BFWAVChannelInfo;

typedef PLSR_ArchiveTable PLSR_BFWAVChannelInfoTable;

typedef struct {
	PLSR_BFWAVFormat format;
	bool looping;
	u32 sampleRate;
	u32 loopStartSample;
	u32 sampleCount;
	PLSR_BFWAVChannelInfoTable channelInfoTable;
} PLSR_BFWAVInfo;

PLSR_RC plsrBFWAVReadChannelInfo(const PLSR_BFWAV* bfwav, const PLSR_BFWAVChannelInfoTable* table, u32 index, PLSR_BFWAVChannelInfo* out);
PLSR_RC plsrBFWAVReadInfo(const PLSR_BFWAV* bfwav, PLSR_BFWAVInfo* out);
