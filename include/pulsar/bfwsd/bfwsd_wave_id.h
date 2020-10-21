#pragma once

#include <pulsar/bfwsd/bfwsd.h>
#include <pulsar/bfsar/bfsar_item.h>

typedef struct {
	PLSR_BFSARItemId archiveItemId;
	u32 index;
} PLSR_BFWSDWaveId;

PLSR_RC plsrBFWSDWaveIdListReadEntries(const PLSR_BFWSD* bfwsd, u32 offset, u32 count, PLSR_BFWSDWaveId* outIds, u32* outReadCount);
PLSR_RC plsrBFWSDWaveIdListGetEntry(const PLSR_BFWSD* bfwsd, u32 offset, PLSR_BFWSDWaveId* out);

NX_INLINE u32 plsrBFWSDWaveIdCount(const PLSR_BFWSD* bfwsd) {
	return bfwsd->waveIdList.info.count;
}
