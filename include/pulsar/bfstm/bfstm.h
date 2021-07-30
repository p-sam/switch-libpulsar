/**
 * @file
 * @brief Stream file init
 */
#pragma once

#include <pulsar/archive/archive.h>
#include <pulsar/bfstm/bfstm_internal.h>

/// Stream file method categories
typedef enum {
	PLSR_BFSTMCategoryType_Init = 0,
	PLSR_BFSTMCategoryType_Info,
	PLSR_BFSTMCategoryType_Channel,
} PLSR_BFSTMCategoryType;

/// Stream file sample formats
typedef enum {
	/// 8-bit PCM encoded samples
	PLSR_BFSTMFormat_PCM_8 = _PLSR_BFSTM_FORMAT_PCM_8,

	/// 16-bit PCM encoded samples
	PLSR_BFSTMFormat_PCM_16 = _PLSR_BFSTM_FORMAT_PCM_16,

	/// DSP ADPCM encoded samples (Wave channel info should contain the needed adpcm context to decode samples, see plsrBFWAVReadChannelInfo())
	PLSR_BFSTMFormat_DSP_ADPCM = _PLSR_BFSTM_FORMAT_DSP_ADPCM,
} PLSR_BFSTMFormat;

/// Stream file
typedef struct {
	PLSR_Archive ar;

	PLSR_ArchiveHeaderInfo headerInfo;

	PLSR_ArchiveSection infoSection;
	PLSR_ArchiveSection dataSection;

	u32 streamInfoOffset;
	PLSR_ArchiveTable channelTable;
} PLSR_BFSTM;

/// @copydoc plsrArchiveOpen
PLSR_RC plsrBFSTMOpen(const char* path, PLSR_BFSTM* out);

/// @copydoc plsrArchiveOpenInside
PLSR_RC plsrBFSTMOpenInside(const PLSR_Archive* ar, u32 offset, PLSR_BFSTM* out);

/// @copydoc plsrArchiveClose
void plsrBFSTMClose(PLSR_BFSTM* bfstm);
