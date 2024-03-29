#ifdef __SWITCH__

#include <pulsar/player/player_load_lookup.h>

#include <pulsar/bfsar/bfsar_string.h>
#include <pulsar/bfsar/bfsar_file.h>
#include <pulsar/bfsar/bfsar_sound.h>
#include <pulsar/bfsar/bfsar_wave_archive.h>
#include <pulsar/bfwar/bfwar.h>
#include <pulsar/bfwar/bfwar_file.h>
#include <pulsar/bfwsd/bfwsd.h>
#include <pulsar/bfwsd/bfwsd_wave_id.h>
#include <pulsar/bfwsd/bfwsd_sound_data.h>
#include <pulsar/bfstm/bfstm.h>
#include <pulsar/player/player_load_formats.h>

#define _LOCAL_RC_MAKE(X) PLSR_RC_MAKE(Player, LoadLookup, X)

static PLSR_RC _loadWaveFromWAR(const PLSR_BFWAR* bfwar, u32 waveIndex, PLSR_PlayerSoundId* out) {
	PLSR_BFWARFileInfo waveFileInfo;
	PLSR_RC_TRY(plsrBFWARFileGet(bfwar, waveIndex, &waveFileInfo));

	PLSR_BFWAV bfwav;
	PLSR_RC_TRY(plsrBFWAVOpenInside(&bfwar->ar, waveFileInfo.offset, &bfwav));
	PLSR_RC rc = plsrPlayerLoadWave(&bfwav, out);
	plsrBFWAVClose(&bfwav);

	return rc;
}

static PLSR_RC _loadWaveFromWSD(const PLSR_BFSAR* bfsar, const PLSR_BFWSD* bfwsd, u32 waveIndex, PLSR_PlayerSoundId* out) {
	PLSR_BFWSDWaveId waveId;

	PLSR_BFWSDSoundDataInfo soundDataInfo;
	PLSR_RC_TRY(plsrBFWSDSoundDataGet(bfwsd, waveIndex, &soundDataInfo));

	PLSR_BFWSDNoteInfo noteInfo;
	PLSR_RC_TRY(plsrBFWSDSoundDataNoteGet(bfwsd, &soundDataInfo.noteInfoTable, 0, &noteInfo));

	PLSR_RC_TRY(plsrBFWSDWaveIdListGetEntry(bfwsd, noteInfo.waveIdIndex, &waveId));
	if(waveId.archiveItemId.type != PLSR_BFSARItemType_WaveArchive) {
		return _LOCAL_RC_MAKE(NotFound);
	}

	PLSR_BFSARWaveArchiveInfo waveArchiveInfo;
	PLSR_RC_TRY(plsrBFSARWaveArchiveGet(bfsar, waveId.archiveItemId.index, &waveArchiveInfo));

	PLSR_BFWAR bfwar;
	PLSR_RC_TRY(plsrBFSARWaveArchiveOpen(bfsar, &waveArchiveInfo, &bfwar));

	PLSR_RC rc = _loadWaveFromWAR(&bfwar, waveId.index, out);
	plsrBFWARClose(&bfwar);

	return rc;
}

static PLSR_RC _loadWaveFromArchive(const PLSR_BFSAR* bfsar, const PLSR_BFSARSoundInfo* soundInfo, const PLSR_BFSARFileInfo* soundFileInfo, PLSR_PlayerSoundId* out) {
	PLSR_BFWSD bfwsd;

	switch(soundFileInfo->type) {
		case PLSR_BFSARFileInfoType_External:
			PLSR_RC_TRY(plsrBFWSDOpen(soundFileInfo->external.path, &bfwsd));
			break;
		case PLSR_BFSARFileInfoType_Internal:
			PLSR_RC_TRY(plsrBFWSDOpenInside(&bfsar->ar, soundFileInfo->internal.offset, &bfwsd));
			break;
		default:
			return _LOCAL_RC_MAKE(Unsupported);
	}

	PLSR_RC rc = _loadWaveFromWSD(bfsar, &bfwsd, soundInfo->wave.index, out);
	plsrBFWSDClose(&bfwsd);

	return rc;
}

static PLSR_RC _loadStreamFromArchive(const PLSR_BFSAR* bfsar, const PLSR_BFSARSoundInfo* soundInfo, const PLSR_BFSARFileInfo* soundFileInfo, PLSR_PlayerSoundId* out) {
	PLSR_BFSTM bfstm;

	switch(soundFileInfo->type) {
		case PLSR_BFSARFileInfoType_External:
			PLSR_RC_TRY(plsrBFSTMOpen(soundFileInfo->external.path, &bfstm));
			break;
		case PLSR_BFSARFileInfoType_Internal:
			PLSR_RC_TRY(plsrBFSTMOpenInside(&bfsar->ar, soundFileInfo->internal.offset, &bfstm));
			break;
		default:
			return _LOCAL_RC_MAKE(Unsupported);
	}

	PLSR_RC rc = plsrPlayerLoadStream(&bfstm, out);
	plsrBFSTMClose(&bfstm);

	return rc;
}

PLSR_RC plsrPlayerLoadSoundByItemId(const PLSR_BFSAR* bfsar, PLSR_BFSARItemId itemId, PLSR_PlayerSoundId* out) {
	if(itemId.type != PLSR_BFSARItemType_Sound) {
		return _LOCAL_RC_MAKE(Unsupported);
	}

	PLSR_BFSARSoundInfo soundInfo;
	PLSR_RC_TRY(plsrBFSARSoundGet(bfsar, itemId.index, &soundInfo));

	PLSR_BFSARFileInfo soundFileInfo;
	PLSR_RC_TRY(plsrBFSARFileScan(bfsar, soundInfo.fileIndex, &soundFileInfo));
	PLSR_RC_TRY(plsrBFSARFileInfoNormalize(bfsar, &soundFileInfo));

	switch(soundInfo.type) {
		case PLSR_BFSARSoundType_Wave:
			return _loadWaveFromArchive(bfsar, &soundInfo, &soundFileInfo, out);
		case PLSR_BFSARSoundType_Stream:
			return _loadStreamFromArchive(bfsar, &soundInfo, &soundFileInfo, out);
		default:
			return _LOCAL_RC_MAKE(Unsupported);
	}
}

PLSR_RC plsrPlayerLoadSoundByName(const PLSR_BFSAR* bfsar, const char* name, PLSR_PlayerSoundId* out) {
	PLSR_BFSARStringSearchInfo searchInfo;

	PLSR_RC_TRY(plsrBFSARStringSearch(bfsar, name, &searchInfo));

	return plsrPlayerLoadSoundByItemId(bfsar, searchInfo.itemId, out);
}

#endif
