/** @example example02-browse.c
 * Lists sounds from a specified sound archive and allows to play them
 */

#include <stdio.h>
#include <switch.h>
#include <pulsar.h>

#define LIST_LINES 30

static PLSR_BFSAR g_bfsar;
static PLSR_PlayerSoundId g_soundId = PLSR_PLAYER_INVALID_SOUND;
static u32 g_selected = 0;

static PLSR_RC _exampleInit(const char* sarPath) {
	// Initialize our player using the default configuration
	PLSR_RC_TRY(plsrPlayerInit());

	// Open the sound archive from the requested path
	PLSR_RC_TRY(plsrBFSAROpen(sarPath, &g_bfsar));

	return PLSR_RC_OK;
}

static void _exampleExit() {
	// Free any loaded sound from memory
	plsrPlayerFree(g_soundId);

	// Close the archive
	plsrBFSARClose(&g_bfsar);

	// De-initialize our player
	plsrPlayerExit();
}

static PLSR_RC _examplePlay() {
	// Stop and free the last loaded sound
	plsrPlayerStop(g_soundId);
	plsrPlayerWaitNextFrame();
	plsrPlayerFree(g_soundId);
	g_soundId = PLSR_PLAYER_INVALID_SOUND;

	// Construct the item id from type and index
	PLSR_BFSARItemId itemId = {
		.type = PLSR_BFSARItemType_Sound,
		.index = g_selected
	};

	// Load and play the selected sound
	PLSR_RC_TRY(plsrPlayerLoadSoundByItemId(&g_bfsar, itemId, &g_soundId));
	PLSR_RC_TRY(plsrPlayerPlay(g_soundId));
	return PLSR_RC_OK;
}

static PLSR_RC _exampleList() {
	// Compute which indexes to display on the screen
	u32 displayMin = g_selected < (LIST_LINES/2) ? 0 : g_selected - (LIST_LINES/2);
	u32 displayMax = displayMin + LIST_LINES;

	// Erase part of the screen, and print indexes
	printf(CONSOLE_ESC(10;1H) CONSOLE_ESC(0J));
	printf("---- %04u / %04u ----\n", g_selected + 1, plsrBFSARSoundCount(&g_bfsar));

	// Retrieve infos to display about each item, and print them
	PLSR_BFSARSoundInfo soundInfo;
	char label[0x100];
	const char* desc;

	for(u32 i = displayMin; i < plsrBFSARSoundCount(&g_bfsar) && i <= displayMax; i++) {
		PLSR_RC_TRY(plsrBFSARSoundGet(&g_bfsar, i, &soundInfo));

		// Fetch the sound name if present
		if(soundInfo.hasStringIndex) {
			PLSR_RC_TRY(plsrBFSARStringGet(&g_bfsar, soundInfo.stringIndex, label, sizeof(label)));
		} else {
			strcpy(label, "???");
		}

		switch(soundInfo.type) {
			case PLSR_BFSARSoundType_Wave:
				desc = "WAV";
				break;
			case PLSR_BFSARSoundType_Sequence:
				desc = "SEQ";
				break;
			case PLSR_BFSARSoundType_Stream:
				desc = "STM";
				break;
			default:
				desc = "???";
				break;
		}

		printf("%c %s [%s]\n", g_selected == i ? '>' : ' ', label, desc);
	}

	printf("---------------------\n");

	return PLSR_RC_OK;
}

int main(int argc, char *argv[]) {
	consoleInit(NULL);
	padConfigureInput(1, HidNpadStyleSet_NpadStandard);

	PadState pad;
	padInitializeDefault(&pad);

	// Mount qlaunch ROMFS storage (just so the sound archive inside can be opened later)
	printf("romfsMountDataStorageFromProgram: 0x%x\n", romfsMountDataStorageFromProgram(0x0100000000001000, "qlaunch"));

	// Initialize resources used in this example
	// We use the qlaunch sound archive unless another one was specified via arguments
	const char* sarPath = argc > 1 ? argv[1] : "qlaunch:/sound/qlaunch.bfsar";
	PLSR_RC initRC = _exampleInit(sarPath);
	printf("sarPath: %s\n", sarPath);
	printf("_exampleInit() = 0x%08X\n\n", initRC);
	printf("Press + to exit\n");

	// If everything is OK, do the initial listing, ignoring its error code
	if(R_SUCCEEDED(initRC)) {
		printf("Press A to play a sound\n");
		printf("Press UP or DOWN to change your selection\n");
		printf("Use the RIGHT STICK to scroll faster\n\n");
		_exampleList();
	}

	// Main loop
	while(appletMainLoop()) {
		padUpdate(&pad);
		u64 kDown = padGetButtonsDown(&pad);
		u64 kHeld = padGetButtons(&pad);

		if(kDown & HidNpadButton_Plus) {
			break;
		}

		if(R_SUCCEEDED(initRC)) {
			if(kDown & HidNpadButton_A) {
				printf("_examplePlay() = 0x%08X\n", _examplePlay());
				// Move cursor up one line
				printf(CONSOLE_ESC(1A));
			}

			if(((kDown & (HidNpadButton_Up | HidNpadButton_StickLUp)) || (kHeld & HidNpadButton_StickRUp)) && g_selected > 0) {
				g_selected--;
				_exampleList();
			}

			if(((kDown & (HidNpadButton_Down | HidNpadButton_StickLDown)) || (kHeld & HidNpadButton_StickRDown)) && g_selected < plsrBFSARSoundCount(&g_bfsar) - 1) {
				g_selected++;
				_exampleList();
			}
		}

		consoleUpdate(NULL);
	}

	// Clean up used resources
	_exampleExit();

	consoleExit(NULL);
	return 0;
}
