#pragma once

#include <pulsar/bfgrp/bfgrp.h>

typedef struct {
	u32 fileIndex;
	u32 fileBlockOffset;
	u32 fileSize;
} PLSR_BFGRPLocationInfo;

PLSR_RC plsrBFGRPLocationGet(const PLSR_BFGRP* bfgrp, u32 index, PLSR_BFGRPLocationInfo* out);
PLSR_RC plsrBFGRPLocationFindByFileIndex(const PLSR_BFGRP* bfgrp, u32 fileIndex, PLSR_BFGRPLocationInfo* out);

NX_INLINE u32 plsrBFGRPLocationCount(const PLSR_BFGRP* bfgrp) {
	return bfgrp->locationTable.info.count;
}
