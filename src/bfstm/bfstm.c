#include <pulsar/bfstm/bfstm.h>

#define _LOCAL_TRY(X) PLSR_RC_LTRY(BFSTM, Init, X)
#define _LOCAL_RC_MAKE(X) PLSR_RC_MAKE(BFSTM, Init, X)

static PLSR_RC _BFSTMReadHeaderInfo(PLSR_BFSTM* bfstm) {
	_LOCAL_TRY(plsrArchiveReadSectionHeaderInfo(&bfstm->ar, bfstm->infoSection.offset, _PLSR_BFSTM_INFO_MAGIC, &bfstm->infoSection.info));

	// Track info table and channel info table references have the same identifier
	// So we read them in order instead of the traditionnal loop
	_PLSR_ArchiveRef _ref;

	_LOCAL_TRY(plsrArchiveRead(&bfstm->ar, &_ref, sizeof(_ref)));
	if(_ref.id != _PLSR_BFSTM_INFO_IDENTIFIER_STREAM_INFO) {
		return _LOCAL_RC_MAKE(Unsupported);
	}
	bfstm->streamInfoOffset = bfstm->infoSection.offset + sizeof(_PLSR_ArchiveSectionHeader) + _ref.offset;

	// UNIMPLEMENTED: track infos
	_LOCAL_TRY(plsrArchiveRead(&bfstm->ar, &_ref, sizeof(_ref)));
	if(_ref.id != _PLSR_BFSTM_INFO_IDENTIFIER_TRACK_TABLE && _ref.id != 0) {
		return _LOCAL_RC_MAKE(Unsupported);
	}

	_LOCAL_TRY(plsrArchiveRead(&bfstm->ar, &_ref, sizeof(_ref)));
	if(_ref.id == _PLSR_BFSTM_INFO_IDENTIFIER_CHANNEL_TABLE) {
		bfstm->channelTable.offset = bfstm->infoSection.offset + sizeof(_PLSR_ArchiveSectionHeader) + _ref.offset;
	} else if(_ref.id != 0) {
		return _LOCAL_RC_MAKE(Unsupported);
	}

	_LOCAL_TRY(plsrArchiveReadTableHeaderInfo(&bfstm->ar, bfstm->channelTable.offset, &bfstm->channelTable.info));

	return PLSR_RC_OK;
}

static PLSR_RC _BFSTMInit(PLSR_BFSTM* bfstm) {
	_LOCAL_TRY(plsrArchiveReadHeaderInfo(&bfstm->ar, _PLSR_BFSTM_MAGIC, &bfstm->headerInfo));

	_PLSR_ArchiveBlockRef _ref;
	unsigned identifierCount = 2;
	for(unsigned i = 0; i < bfstm->headerInfo.sectionCount && identifierCount > 0; i++) {
		_LOCAL_TRY(plsrArchiveRead(&bfstm->ar, &_ref, sizeof(_ref)));

		switch(_ref.id) {
			case _PLSR_BFSTM_SECTION_IDENTIFIER_INFO:
				bfstm->infoSection.offset = _ref.offset;
				identifierCount--;
				break;
			case _PLSR_BFSTM_SECTION_IDENTIFIER_DATA:
				bfstm->dataSection.offset = _ref.offset;
				identifierCount--;
				break;
		}
	}

	// UNIMPLEMENTED: SEEK infos

	_LOCAL_TRY(_BFSTMReadHeaderInfo(bfstm));
	_LOCAL_TRY(plsrArchiveReadSectionHeaderInfo(&bfstm->ar, bfstm->dataSection.offset, _PLSR_BFSTM_DATA_MAGIC, &bfstm->dataSection.info));

	return PLSR_RC_OK;
}

PLSR_RC plsrBFSTMOpen(const char* path, PLSR_BFSTM* out) {
	memset(out, 0, sizeof(PLSR_BFSTM));

	PLSR_RC rc = plsrArchiveOpen(path, &out->ar);

	if(PLSR_RC_SUCCEEDED(rc)) {
		rc = _BFSTMInit(out);
	}

	if(PLSR_RC_FAILED(rc)) {
		plsrBFSTMClose(out);
	}

	return rc;
}

PLSR_RC plsrBFSTMOpenInside(const PLSR_Archive* ar, u32 offset, PLSR_BFSTM* out) {
	memset(out, 0, sizeof(PLSR_BFSTM));

	PLSR_RC rc = plsrArchiveOpenInside(ar, offset, &out->ar);

	if(PLSR_RC_SUCCEEDED(rc)) {
		rc = _BFSTMInit(out);
	}

	if(PLSR_RC_FAILED(rc)) {
		plsrBFSTMClose(out);
	}

	return rc;
}

void plsrBFSTMClose(PLSR_BFSTM* bfstm) {
	plsrArchiveClose(&bfstm->ar);
}
