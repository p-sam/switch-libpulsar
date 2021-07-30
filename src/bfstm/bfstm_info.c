#include <pulsar/bfstm/bfstm_info.h>

#define _LOCAL_TRY(X) PLSR_RC_LTRY(BFSTM, Info, X)
#define _LOCAL_RC_MAKE(X) PLSR_RC_MAKE(BFSTM, Info, X)

PLSR_RC plsrBFSTMReadInfo(const PLSR_BFSTM* bfstm, PLSR_BFSTMInfo* out) {
	_PLSR_BFSTMInfo _info;

	_LOCAL_TRY(plsrArchiveReadAt(&bfstm->ar, bfstm->streamInfoOffset, &_info, sizeof(_info)));

	if(_info.dataRef.id != _PLSR_BFSTM_INFO_IDENTIFIER_STREAM_DATA) {
		return _LOCAL_RC_MAKE(NotFound);
	}

	out->format = _info.format;
	out->looping = _info.looping;
	out->sampleRate = _info.sampleRate;
	out->loopStartSample = _info.loopStartSample;
	out->sampleCount = _info.sampleCount;
	out->blockCount = _info.blockCount;
	out->blockSize = _info.blockSize;
	out->blockSampleCount = _info.blockSampleCount;
	out->lastBlockSize = _info.lastBlockSize;
	out->lastBlockSampleCount = _info.lastBlockSampleCount;
	out->lastBlockSizeWithPadding = _info.lastBlockSizeWithPadding;

	out->dataOffset = _info.dataRef.offset + bfstm->dataSection.offset + sizeof(_PLSR_ArchiveSectionHeader);

	// UNIMPLEMENTED: SEEK infos
	return PLSR_RC_OK;
}
