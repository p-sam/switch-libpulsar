#pragma once

#include <pulsar/bfsar/bfsar.h>
#include <pulsar/bfsar/bfsar_item.h>

typedef struct {
	char name[FS_MAX_PATH];
	PLSR_BFSARItemId itemId;
} PLSR_BFSARStringSearchInfo;

PLSR_RC plsrBFSARStringGet(const PLSR_BFSAR* bfsar, u32 index, char* out, size_t size);
PLSR_RC plsrBFSARStringSearchEx(const PLSR_BFSAR* bfsar, const char* query, size_t queryLen, PLSR_BFSARStringSearchInfo* out);
NX_INLINE PLSR_RC plsrBFSARStringSearch(const PLSR_BFSAR* bfsar, const char* query, PLSR_BFSARStringSearchInfo* out) {
	return plsrBFSARStringSearchEx(bfsar, query, strlen(query), out);
}

NX_INLINE u32 plsrBFSARStringCount(const PLSR_BFSAR* bfsar) {
	return bfsar->stringTable.info.count;
}
