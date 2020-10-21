#pragma once

#include <pulsar/archive/archive.h>
#include <pulsar/bfwsd/bfwsd_internal.h>

typedef enum {
	PLSR_BFWSDCategoryType_Init = 0,
	PLSR_BFWSDCategoryType_WaveId,
	PLSR_BFWSDCategoryType_SoundData,
} PLSR_BFWSDCategoryType;

typedef struct {
	PLSR_Archive ar;

	PLSR_ArchiveHeaderInfo headerInfo;

	PLSR_ArchiveSection infoSection;

	PLSR_ArchiveList waveIdList;
	PLSR_ArchiveTable soundDataTable;
} PLSR_BFWSD;

PLSR_RC plsrBFWSDOpen(const char* path, PLSR_BFWSD* out);
PLSR_RC plsrBFWSDOpenInside(const PLSR_Archive* ar, u32 offset, PLSR_BFWSD* out);
void plsrBFWSDClose(PLSR_BFWSD* bfwsd);
