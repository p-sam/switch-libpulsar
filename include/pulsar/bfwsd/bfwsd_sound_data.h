#pragma once

#include <pulsar/bfwsd/bfwsd.h>

typedef PLSR_ArchiveTable PLSR_BFWSDNoteInfoTable;

typedef struct {
	u8 attack;
	u8 decay;
	u8 sustain;
	u8 hold;
	u8 release;
} PLSR_BFWSDAdshrCurve;

typedef struct {
	u8 main;
	u8 auxCount;
	u8 aux[3];
} PLSR_BFWSDSendValues;

typedef struct {
	u32 waveIdIndex;
} PLSR_BFWSDNoteInfo;

typedef struct {
	PLSR_BFWSDNoteInfoTable noteInfoTable;

	bool hasPan;
	u8 pan;
	u8 surroundPan;

	bool hasPitch;
	float pitch;

	bool hasSend;
	PLSR_BFWSDSendValues send;

	bool hasAdshr;
	PLSR_BFWSDAdshrCurve adshr;

} PLSR_BFWSDSoundDataInfo;

PLSR_RC plsrBFWSDSoundDataGet(const PLSR_BFWSD* bfwsd, u32 index, PLSR_BFWSDSoundDataInfo* out);
PLSR_RC plsrBFWSDSoundDataNoteGet(const PLSR_BFWSD* bfwsd, const PLSR_BFWSDNoteInfoTable* table, u32 index, PLSR_BFWSDNoteInfo* out);

NX_INLINE u32 plsrBFWSDSoundDataCount(const PLSR_BFWSD* bfwsd) {
	return bfwsd->soundDataTable.info.count;
}
