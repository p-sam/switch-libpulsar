#pragma once

#include <pulsar/bfsar/bfsar.h>
#include <pulsar/bfgrp/bfgrp.h>

typedef struct {
	u32 fileIndex;

	bool hasStringIndex;
	u32 stringIndex;
} PLSR_BFSARGroupInfo;

PLSR_RC plsrBFSARGroupGet(const PLSR_BFSAR* bfsar, u32 index, PLSR_BFSARGroupInfo* out);
PLSR_RC plsrBFSARGroupOpen(const PLSR_BFSAR* bfsar, const PLSR_BFSARGroupInfo* groupInfo, PLSR_BFGRP* out);

NX_INLINE u32 plsrBFSARGroupCount(const PLSR_BFSAR* bfsar) {
	return bfsar->groupTable.info.count;
}
