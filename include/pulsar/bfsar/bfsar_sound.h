#pragma once

#include <pulsar/bfsar/bfsar.h>
#include <pulsar/bfsar/bfsar_item.h>

typedef enum {
	PLSR_BFSARSoundType_Stream = _PLSR_BFSAR_INFO_IDENTIFIER_STREAM_ENTRY,
	PLSR_BFSARSoundType_Wave = _PLSR_BFSAR_INFO_IDENTIFIER_WAVE_ENTRY,
	PLSR_BFSARSoundType_Sequence = _PLSR_BFSAR_INFO_IDENTIFIER_SEQUENCE_ENTRY,
} PLSR_BFSARSoundType;

typedef struct {
	u32 index;
	u32 trackCount;
} PLSR_BFSARSoundWaveInfo;

typedef struct {
	u32 fileIndex;
	PLSR_BFSARItemId playerItemId;
	u8 initialVolume;
	u8 remoteFilter;
	PLSR_BFSARSoundType type;

	PLSR_BFSARSoundWaveInfo wave;

	bool hasStringIndex;
	u32 stringIndex;
} PLSR_BFSARSoundInfo;

PLSR_RC plsrBFSARSoundGet(const PLSR_BFSAR* bfsar, u32 index, PLSR_BFSARSoundInfo* out);

NX_INLINE u32 plsrBFSARSoundCount(const PLSR_BFSAR* bfsar) {
	return bfsar->soundTable.info.count;
}
