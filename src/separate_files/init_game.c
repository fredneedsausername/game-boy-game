#include "separate_files.h"

void init_game(void) {
	// --- Set game-wide values ---
	SPRITES_8x8;
	SHOW_SPRITES;
	SHOW_BKG;
	mode(get_mode() | M_NO_SCROLL); // Turn off scrolling
	init_fred_arand((uint16_t) 42069);
}