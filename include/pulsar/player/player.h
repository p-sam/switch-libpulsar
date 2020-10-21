#pragma once

#include <pulsar/archive/archive.h>
#include <pulsar/bfwav/bfwav.h>
#include <pulsar/bfwav/bfwav_info.h>

#define PLSR_PLAYER_SINK_CHANNELS 2
#define PLSR_PLAYER_INVALID_SOUND NULL

typedef enum {
	PLSR_PlayerCategoryType_Init = 0,
	PLSR_PlayerCategoryType_Load,
	PLSR_PlayerCategoryType_LoadLookup,
} PLSR_PlayerCategoryType;

typedef struct {
	bool initRenderer;
	AudioRendererConfig audrenConfig;
	int startVoiceId;
	int endVoiceId;
	const u8 sinkChannels[PLSR_PLAYER_SINK_CHANNELS];
} PLSR_PlayerConfig;

typedef struct {
	void* mempool;
	AudioDriverWaveBuf wavebuf;
	int mempoolId;
	int voiceId;
} PLSR_PlayerSoundChannel;

typedef struct {
	unsigned int channelCount;
	PLSR_PlayerSoundChannel channels[PLSR_PLAYER_SINK_CHANNELS];
} PLSR_PlayerSound;

typedef const PLSR_PlayerSound* PLSR_PlayerSoundId;

typedef struct {
	AudioDriver driver;
	PLSR_PlayerConfig config;
} PLSR_Player;

const PLSR_PlayerConfig* plsrPlayerGetDefaultConfig(void);
PLSR_Player* plsrPlayerGetInstance(void);

PLSR_RC plsrPlayerInitEx(const PLSR_PlayerConfig* config);
NX_INLINE PLSR_RC plsrPlayerInit(void) {
	return plsrPlayerInitEx(plsrPlayerGetDefaultConfig());
}

NX_INLINE void plsrPlayerWaitNextFrame(void) {
	audrenWaitFrame();
}

void plsrPlayerDestroy(void);

PLSR_RC plsrPlayerPlay(PLSR_PlayerSoundId id);
PLSR_RC plsrPlayerStop(PLSR_PlayerSoundId id);
void plsrPlayerFree(PLSR_PlayerSoundId id);