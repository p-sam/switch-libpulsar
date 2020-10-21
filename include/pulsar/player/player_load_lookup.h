#pragma once

#include <pulsar/player/player.h>
#include <pulsar/bfsar/bfsar.h>
#include <pulsar/bfsar/bfsar_item.h>

PLSR_RC plsrPlayerLoadSoundByName(const PLSR_BFSAR* bfsar, const char* name, PLSR_PlayerSoundId* out);
PLSR_RC plsrPlayerLoadSoundByItemId(const PLSR_BFSAR* bfsar, PLSR_BFSARItemId itemId, PLSR_PlayerSoundId* out);
