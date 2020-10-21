#pragma once

#include <pulsar/archive/archive.h>
#include <pulsar/bfsar/bfsar_internal.h>

typedef enum {
	PLSR_BFSARCategoryType_Init = 0,
	PLSR_BFSARCategoryType_IdList,
	PLSR_BFSARCategoryType_String,
	PLSR_BFSARCategoryType_Sound,
	PLSR_BFSARCategoryType_WaveArchive,
	PLSR_BFSARCategoryType_Group,
	PLSR_BFSARCategoryType_File,
} PLSR_BFSARCategoryType;

typedef struct {
	u32 rootNodeIndex;
	u32 nodeCount;
} PLSR_BFSARStringTreeInfo;

typedef struct {
	u32 offset;
	PLSR_BFSARStringTreeInfo info;
} PLSR_BFSARStringTree;

typedef struct {
	PLSR_Archive ar;

	PLSR_ArchiveHeaderInfo headerInfo;

	PLSR_ArchiveSection strgSection;
	PLSR_ArchiveSection infoSection;
	PLSR_ArchiveSection fileSection;

	PLSR_ArchiveTable stringTable;
	PLSR_BFSARStringTree stringTree;

	PLSR_ArchiveTable soundTable;
	PLSR_ArchiveTable waveArchiveTable;
	PLSR_ArchiveTable groupTable;
	PLSR_ArchiveTable fileTable;
} PLSR_BFSAR;

PLSR_RC plsrBFSAROpen(const char* path, PLSR_BFSAR* out);
void plsrBFSARClose(PLSR_BFSAR* bfsar);
