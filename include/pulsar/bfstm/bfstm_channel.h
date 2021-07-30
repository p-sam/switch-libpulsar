/**
 * @file
 * @brief Stream file channel info
 */
#pragma once

#include <pulsar/bfstm/bfstm.h>

/// Stream channel ADPCM context information
typedef struct {
	u16 header; ///< Initial header (predictor/scale)
	s16 yn1; ///< Sample history 1
	s16 yn2; ///< Sample history 2
} PLSR_BFSTMChannelAdpcmContextInfo;

/// Stream channel ADPCM information (coefficients and contexts)
typedef struct {
	u16 coeffs[16]; ///< ADPCM coefficients
	PLSR_BFSTMChannelAdpcmContextInfo main; ///< ADPCM main context
	PLSR_BFSTMChannelAdpcmContextInfo loop; ///< ADPCM loop context (if stream info `looping` flag is set)
} PLSR_BFSTMChannelAdpcmInfo;

/// Stream channel information
typedef struct {
	PLSR_BFSTMChannelAdpcmInfo adpcmInfo; ///< ADPCM information (if stream sample format is PLSR_BFSTMFormat_DSP_ADPCM)
} PLSR_BFSTMChannelInfo;

/// Fetch channel information from the specified index in the table
PLSR_RC plsrBFSTMChannelGet(const PLSR_BFSTM* bfstm, u32 index, PLSR_BFSTMChannelInfo* out);

NX_INLINE u32 plsrBFSTMChannelCount(const PLSR_BFSTM* bfstm) {
	return bfstm->channelTable.info.count;
}

