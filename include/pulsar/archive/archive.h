#pragma once

#include <pulsar/types.h>
#include <pulsar/archive/archive_internal.h>
#include <pulsar/archive/archive_file.h>

typedef union {
	u32 raw;
	struct {
		u8 revision;
		u8 minor;
		u8 major;
	};
} PLSR_ArchiveVersion;

typedef struct {
	PLSR_ArchiveVersion version;
	u32 fileSize;
	u16 sectionCount;
} PLSR_ArchiveHeaderInfo;

typedef struct {
	u32 blockSize;
} PLSR_ArchiveSectionInfo;

typedef struct {
	u32 offset;
	PLSR_ArchiveSectionInfo info;
} PLSR_ArchiveSection;

typedef struct {
	u32 count;
} PLSR_ArchiveTableInfo;

typedef struct {
	u32 offset;
	PLSR_ArchiveTableInfo info;
} PLSR_ArchiveTable;

typedef struct {
	u32 offset;
} PLSR_ArchiveTableEntry;

typedef struct {
	u32 offset;
	u32 size;
} PLSR_ArchiveTableBlock;

typedef struct {
	u32 entrySize;
	u32 count;
} PLSR_ArchiveListInfo;

typedef struct {
	u32 offset;
	PLSR_ArchiveListInfo info;
} PLSR_ArchiveList;

typedef struct {
	PLSR_ArchiveFileHandle handle;
	u32 offset;
} PLSR_Archive;

PLSR_RC plsrArchiveOpen(const char* path, PLSR_Archive* out);
PLSR_RC plsrArchiveOpenInside(const PLSR_Archive* ar, u32 offset, PLSR_Archive* out);
void plsrArchiveClose(PLSR_Archive* ar);

NX_INLINE PLSR_RC plsrArchiveRead(const PLSR_Archive* ar, void* out, size_t size) {
	return plsrArchiveFileRead(ar->handle, out, size) ? PLSR_RC_OK : PLSR_ResultType_NotFound;
}

PLSR_RC plsrArchiveReadAtEx(const PLSR_Archive* ar, u32 offset, void* out, size_t size, bool acceptZero);
NX_INLINE PLSR_RC plsrArchiveReadAt(const PLSR_Archive* ar, u32 offset, void* out, size_t size) {
	return plsrArchiveReadAtEx(ar, offset, out, size, false);
}
PLSR_RC plsrArchiveReadString(const PLSR_Archive* ar, u32 offset, char* out, size_t size);

PLSR_RC plsrArchiveReadHeaderInfo(const PLSR_Archive* ar, const char* magic, PLSR_ArchiveHeaderInfo* out);
PLSR_RC plsrArchiveReadSectionHeaderInfo(const PLSR_Archive* ar, u32 offset, const char* magic, PLSR_ArchiveSectionInfo* out);
PLSR_RC plsrArchiveReadTableHeaderInfo(const PLSR_Archive* ar, u32 offset, PLSR_ArchiveTableInfo* out);

PLSR_RC plsrArchiveReadTableEntry(const PLSR_Archive* ar, const PLSR_ArchiveTable* table, u16 id, u32 index, PLSR_ArchiveTableEntry* out);
PLSR_RC plsrArchiveReadTableBlock(const PLSR_Archive* ar, const PLSR_ArchiveTable* table, u16 id, u32 index, PLSR_ArchiveTableBlock* out);

PLSR_RC plsrArchiveReadListEntries(const PLSR_Archive* ar, const PLSR_ArchiveList* list, u32 offset, u32 count, void* outEntries, u32* outReadCount);
PLSR_RC plsrArchiveListGetEntry(const PLSR_Archive* ar, const PLSR_ArchiveList* list, u32 offset, void* out);
