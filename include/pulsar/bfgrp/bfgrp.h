#pragma once

#include <pulsar/archive/archive.h>
#include <pulsar/bfgrp/bfgrp_internal.h>

typedef enum {
	PLSR_BFGRPCategoryType_Init = 0,
	PLSR_BFGRPCategoryType_Location,
} PLSR_BFGRPCategoryType;

typedef struct {
	PLSR_Archive ar;

	PLSR_ArchiveHeaderInfo headerInfo;

	PLSR_ArchiveSection infoSection;
	PLSR_ArchiveSection fileSection;

	PLSR_ArchiveTable locationTable;
} PLSR_BFGRP;

PLSR_RC plsrBFGRPOpen(const char* path, PLSR_BFGRP* out);
PLSR_RC plsrBFGRPOpenInside(const PLSR_Archive* ar, u32 offset, PLSR_BFGRP* out);
void plsrBFGRPClose(PLSR_BFGRP* bfgrp);
