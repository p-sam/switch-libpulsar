#include <pulsar/bfstm/bfstm_channel.h>

#define _LOCAL_TRY(X) PLSR_RC_LTRY(BFSTM, Channel, X)
#define _LOCAL_RC_MAKE(X) PLSR_RC_MAKE(BFSTM, Channel, X)

PLSR_RC plsrBFSTMChannelGet(const PLSR_BFSTM* bfstm, u32 index, PLSR_BFSTMChannelInfo* out) {
	PLSR_ArchiveTableEntry tableEntry;
	_LOCAL_TRY(plsrArchiveReadTableEntry(&bfstm->ar, &bfstm->channelTable, _PLSR_BFSTM_INFO_IDENTIFIER_CHANNEL_ENTRY, index, &tableEntry));

	_PLSR_BFSTMChannelInfo _channelInfo;
	_LOCAL_TRY(plsrArchiveReadAt(&bfstm->ar, tableEntry.offset, &_channelInfo, sizeof(_channelInfo)));

	if(_channelInfo.adpcmInfoRef.id == _PLSR_BFSTM_INFO_IDENTIFIER_ADPCM_ENTRY) {
		_LOCAL_TRY(plsrArchiveReadAt(&bfstm->ar, tableEntry.offset + _channelInfo.adpcmInfoRef.offset, &out->adpcmInfo, sizeof(out->adpcmInfo)));
	}

	return PLSR_RC_OK;
}
