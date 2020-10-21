#pragma once

#include <pulsar/bfsar/bfsar.h>

typedef enum {
	PLSR_BFSARItemType_Unknown = 0,
	PLSR_BFSARItemType_Sound = _PLSR_BFSAR_FILE_TYPE_SOUND,
	PLSR_BFSARItemType_SoundSet = _PLSR_BFSAR_FILE_TYPE_SOUND_SET,
	PLSR_BFSARItemType_Bank = _PLSR_BFSAR_FILE_TYPE_BANK,
	PLSR_BFSARItemType_Player = _PLSR_BFSAR_FILE_TYPE_PLAYER,
	PLSR_BFSARItemType_WaveArchive = _PLSR_BFSAR_FILE_TYPE_WAVE_ARCHIVE,
	PLSR_BFSARItemType_Group = _PLSR_BFSAR_FILE_TYPE_GROUP,
} PLSR_BFSARItemType;

typedef union {
	uint32_t raw;
	struct {
		uint32_t index : _PLSR_BFSAR_ITEM_ID_INDEX_BITS;
		PLSR_BFSARItemType type : _PLSR_BFSAR_ITEM_ID_TYPE_BITS;
	};
} PLSR_BFSARItemId;
