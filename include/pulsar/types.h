#pragma once

#include <switch.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#define PLSR_RC_MAKE_RESULT_TYPE(RESULT_TYPE) (PLSR_ResultType_ ## RESULT_TYPE)
#define PLSR_RC_MAKE_CATEGORY_TYPE(ARCHIVE_TYPE, CATEGORY_TYPE) (PLSR_ ## ARCHIVE_TYPE ## CategoryType_ ## CATEGORY_TYPE)
#define PLSR_RC_MAKE_ARCHIVE_TYPE(ARCHIVE_TYPE) (PLSR_ArchiveType_ ## ARCHIVE_TYPE)

#define PLSR_RC_MAKE_RAW(ARCHIVE, CATEGORY, RESULT) (RESULT + (CATEGORY << 8) + (ARCHIVE << 16))

#define PLSR_RC_MAKE(ARCHIVE_TYPE, CATEGORY_TYPE, RESULT_TYPE) PLSR_RC_MAKE_RAW(PLSR_RC_MAKE_ARCHIVE_TYPE(ARCHIVE_TYPE), PLSR_RC_MAKE_CATEGORY_TYPE(ARCHIVE_TYPE, CATEGORY_TYPE), PLSR_RC_MAKE_RESULT_TYPE(RESULT_TYPE))
 
#define PLSR_RC_OK 0
#define PLSR_RC_FAILED(RESULT) (RESULT != PLSR_RC_OK)
#define PLSR_RC_SUCCEEDED(RESULT) (RESULT == PLSR_RC_OK)

#define PLSR_RC_ARCHIVE(RESULT) ((RESULT >> 16) & 0xFF)
#define PLSR_RC_CATEGORY(RESULT) ((RESULT >> 8) & 0xFF)
#define PLSR_RC_RESULT(RESULT) (RESULT & 0xFF)

#define PLSR_RC_CONVERT(RESULT, NEW_ARCHIVE_TYPE, NEW_CATEGORY_TYPE) PLSR_RC_MAKE_RAW(PLSR_RC_MAKE_ARCHIVE_TYPE(NEW_ARCHIVE_TYPE), PLSR_RC_MAKE_CATEGORY_TYPE(NEW_ARCHIVE_TYPE, NEW_CATEGORY_TYPE), PLSR_RC_RESULT(RESULT))

#define PLSR_RC_TRY(X) do {\
	const PLSR_RC _rc = (X);\
	if(PLSR_RC_FAILED(_rc)) { return _rc; }\
} while(false)

#define PLSR_RC_LTRY(ARCHIVE_TYPE, CATEGORY_TYPE, X) do {\
	const PLSR_RC _rc = (X);\
	if(PLSR_RC_FAILED(_rc)) { return PLSR_RC_CONVERT(_rc, ARCHIVE_TYPE, CATEGORY_TYPE); }\
} while(false)

#define PLSR_RC_NX_LTRY(ARCHIVE_TYPE, CATEGORY_TYPE, NX_RESULT) if(R_FAILED(NX_RESULT)) { return PLSR_RC_MAKE(ARCHIVE_TYPE, CATEGORY_TYPE, System); }

typedef u32 PLSR_RC;

typedef enum {
	PLSR_ResultType_OK = 0,
	PLSR_ResultType_FileRead,
	PLSR_ResultType_BadMagic,
	PLSR_ResultType_BadEndianness,
	PLSR_ResultType_BadInput,
	PLSR_ResultType_NotFound,
	PLSR_ResultType_NotReady,
	PLSR_ResultType_Unsupported,
	PLSR_ResultType_Memory,

	PLSR_ResultType_System = 0xFF
} PLSR_ResultType;

typedef enum {
	PLSR_ArchiveType_Unknown = 0,
	PLSR_ArchiveType_BFSAR,
	PLSR_ArchiveType_BFGRP,
	PLSR_ArchiveType_BFWSD,
	PLSR_ArchiveType_BFWAR,
	PLSR_ArchiveType_BFWAV,

	PLSR_ArchiveType_Player = 0xFF,
} PLSR_ArchiveType;
