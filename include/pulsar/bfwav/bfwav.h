#pragma once

#include <pulsar/archive/archive.h>
#include <pulsar/bfwav/bfwav_internal.h>

typedef enum {
	PLSR_BFWAVCategoryType_Init = 0,
	PLSR_BFWAVCategoryType_Info,
	PLSR_BFWAVCategoryType_Data,
} PLSR_BFWAVCategoryType;

typedef enum {
	PLSR_BFWAVFormat_PCM_8 = _PLSR_BFWAV_FORMAT_PCM_8,
	PLSR_BFWAVFormat_PCM_16 = _PLSR_BFWAV_FORMAT_PCM_16,
	PLSR_BFWAVFormat_DSP_ADPCM = _PLSR_BFWAV_FORMAT_DSP_ADPCM,
} PLSR_BFWAVFormat;

typedef struct {
	PLSR_Archive ar;

	PLSR_ArchiveHeaderInfo headerInfo;

	PLSR_ArchiveSection infoSection;
	PLSR_ArchiveSection dataSection;
} PLSR_BFWAV;

PLSR_RC plsrBFWAVOpen(const char* path, PLSR_BFWAV* out);
PLSR_RC plsrBFWAVOpenInside(const PLSR_Archive* ar, u32 offset, PLSR_BFWAV* out);
void plsrBFWAVClose(PLSR_BFWAV* bfwav);
