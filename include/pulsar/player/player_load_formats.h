/**
 * @file
 * @brief Player sound archive formats support
 */
#pragma once

#include <pulsar/player/player.h>
#include <pulsar/bfwav/bfwav.h>
#include <pulsar/bfstm/bfstm.h>

/// Load a sound from a Wave file
PLSR_RC plsrPlayerLoadWave(const PLSR_BFWAV* bfwav, PLSR_PlayerSoundId* out);

/// Load a sound from a Stream file
PLSR_RC plsrPlayerLoadStream(const PLSR_BFSTM* bfstm, PLSR_PlayerSoundId* out);
