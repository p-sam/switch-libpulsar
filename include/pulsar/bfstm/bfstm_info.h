/**
 * @file
 * @brief Stream file info
 */
#pragma once

#include <pulsar/bfstm/bfstm.h>

/// Stream information
typedef struct {
	PLSR_BFSTMFormat format; ///< Sample format
	bool looping; ///< `true` if the file contains a looping section
	u32 sampleRate; ///< Sample rate
	u32 loopStartSample; ///< Sample index where the loop starts (if looping is `true`)
	u32 sampleCount; ///< Sample count
	u32 dataOffset; ///< Offset to first channel sample data relative to the start of the file (see plsrArchiveReadAt() to read)

	u32 blockCount; ///< Block count
	u32 blockSize; /// Size of one block
	u32 blockSampleCount; /// Sample count in one block
	u32 lastBlockSize; /// Size of the last block
	u32 lastBlockSampleCount; /// Sample count of the last block
	u32 lastBlockSizeWithPadding; /// Padded size of the last block
} PLSR_BFSTMInfo;

/// Read wave file information from header
PLSR_RC plsrBFSTMReadInfo(const PLSR_BFSTM* bfstm, PLSR_BFSTMInfo* out);

