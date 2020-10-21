#pragma once

#include <pulsar/archive/archive.h>
#include <pulsar/bfwar/bfwar_internal.h>

typedef enum {
	PLSR_BFWARCategoryType_Init = 0,
	PLSR_BFWARCategoryType_File,
} PLSR_BFWARCategoryType;

typedef struct {
	PLSR_Archive ar;

	PLSR_ArchiveHeaderInfo headerInfo;

	PLSR_ArchiveSection infoSection;
	PLSR_ArchiveSection fileSection;

	PLSR_ArchiveTable fileTable;
} PLSR_BFWAR;

PLSR_RC plsrBFWAROpen(const char* path, PLSR_BFWAR* out);
PLSR_RC plsrBFWAROpenInside(const PLSR_Archive* ar, u32 offset, PLSR_BFWAR* out);
void plsrBFWARClose(PLSR_BFWAR* bfwar);
