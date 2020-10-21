#pragma once

#include <pulsar/bfsar/bfsar.h>

typedef enum {
	PLSR_BFSARFileInfoType_Unknown = 0,
	PLSR_BFSARFileInfoType_Internal = _PLSR_BFSAR_FILE_IDENTIFIER_INTERNAL,
	PLSR_BFSARFileInfoType_External = _PLSR_BFSAR_FILE_IDENTIFIER_EXTERNAL,
} PLSR_BFSARFileInfoType;

typedef struct {
	char path[FS_MAX_PATH];
} PLSR_BFSARFileInfoExternal;

typedef struct {
	bool valid;
	u16 id;
	u32 offset;
	u32 size;
} PLSR_BFSARFileInfoInternal;

typedef struct {
	PLSR_BFSARFileInfoType type;
	union {
		PLSR_BFSARFileInfoInternal internal;
		PLSR_BFSARFileInfoExternal external;
	};
	bool fromGroup;
	u32 groupIndex;
} PLSR_BFSARFileInfo;

PLSR_RC plsrBFSARFileGet(const PLSR_BFSAR* bfsar, u32 index, PLSR_BFSARFileInfo* out);
PLSR_RC plsrBFSARFileScan(const PLSR_BFSAR* bfsar, u32 index, PLSR_BFSARFileInfo* out);

NX_INLINE u32 plsrBFSARFileCount(const PLSR_BFSAR* bfsar) {
	return bfsar->fileTable.info.count;
}
