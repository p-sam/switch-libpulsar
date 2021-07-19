/** @example example01-simple.c
 * Basic example, loads and play a sound from qlaunch sound archive
 */

#include <stdio.h>
#include <switch.h>
#include <pulsar.h>

static PLSR_BFSAR g_bfsar;
static PLSR_PlayerSoundId g_soundId = PLSR_PLAYER_INVALID_SOUND;

static PLSR_RC _exampleInit() {
	// Initialize our player using the default configuration
	PLSR_RC_TRY(plsrPlayerInit());

	// Open the sound archive from qlaunch sound archive
	PLSR_RC_TRY(plsrBFSAROpen("qlaunch:/sound/qlaunch.bfsar", &g_bfsar));

	// Load the sound in memory from the previously opened archive
	PLSR_RC_TRY(plsrPlayerLoadSoundByName(&g_bfsar, "SeUnlockKeyZR", &g_soundId));
	return PLSR_RC_OK;
}

static void _exampleExit() {
	// Free the sound from memory
	plsrPlayerFree(g_soundId);

	// Close the archive
	plsrBFSARClose(&g_bfsar);

	// De-initialize our player
	plsrPlayerExit();
}

static PLSR_RC _examplePlay(float pitch, float volume) {
	// Set the requested pitch and volume factors
	plsrPlayerSetPitch(g_soundId, pitch);
	plsrPlayerSetVolume(g_soundId, volume);

	// Play the previously loaded sound
	return plsrPlayerPlay(g_soundId);
}

int main() {
	consoleInit(NULL);
	padConfigureInput(1, HidNpadStyleSet_NpadStandard);

	PadState pad;
	padInitializeDefault(&pad);

	// Mount qlaunch ROMFS storage (so that the sound archive can be read)
	printf("romfsMountDataStorageFromProgram: 0x%x\n", romfsMountDataStorageFromProgram(0x0100000000001000, "qlaunch"));

	// Initialize resources used in this example
	PLSR_RC initRC = _exampleInit();
	printf("_exampleInit() = 0x%08X\n\n", initRC);
	printf("Press + to exit\n");

	if(R_SUCCEEDED(initRC)) {
		printf("Change pitch with the left stick Y axis\n");
		printf("Change volume with the right stick Y axis\n");
		printf("Press A to play a sound\n");
	}

	// Main loop
	while(appletMainLoop()) {
		padUpdate(&pad);
		u64 kDown = padGetButtonsDown(&pad);

		if(kDown & HidNpadButton_Plus) {
			break;
		}

		if(R_SUCCEEDED(initRC) && (kDown & HidNpadButton_A)) {
			HidAnalogStickState analog_stick_l = padGetStickPos(&pad, 0);
			HidAnalogStickState analog_stick_r = padGetStickPos(&pad, 1);
			float pitch = 1.0f + (float)analog_stick_l.y / (float)JOYSTICK_MAX;
			float volume = 1.0f + (float)analog_stick_r.y / (float)JOYSTICK_MAX;

			if(pitch <= 0.05f) {
				pitch = 0.05f;
			}

			printf("_examplePlay(%f, %f) = 0x%08X\n", pitch, volume, _examplePlay(pitch, volume));
		}

		consoleUpdate(NULL);
	}

	// Clean up used resources
	_exampleExit();

	consoleExit(NULL);
	return 0;
}
