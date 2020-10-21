#pragma once

#include <pulsar/bfsar/bfsar.h>
#include <pulsar/bfwar/bfwar.h>

typedef struct {
	u32 fileIndex;

	bool hasStringIndex;
	u32 stringIndex;

	bool hasWaveCount;
	u32 waveCount;
} PLSR_BFSARWaveArchiveInfo;

PLSR_RC plsrBFSARWaveArchiveGet(const PLSR_BFSAR* bfsar, u32 index, PLSR_BFSARWaveArchiveInfo* out);
PLSR_RC plsrBFSARWaveArchiveOpen(const PLSR_BFSAR* bfsar, const PLSR_BFSARWaveArchiveInfo* waveArchiveInfo, PLSR_BFWAR* out);

NX_INLINE u32 plsrBFSARWaveArchiveCount(const PLSR_BFSAR* bfsar) {
	return bfsar->waveArchiveTable.info.count;
}
