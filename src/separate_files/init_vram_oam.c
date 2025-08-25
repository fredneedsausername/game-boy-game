#include "separate_files.h"


void init_vram_oam(void) {
    disable_interrupts(); 	// Don't slow CPU down while loading
	DISPLAY_OFF;			// Safe loading into VRAM and OAM without corruption

	// --- Start loading into VRAM and OAM ---

	// Load game HUD
	set_bkg_data(0, game_background_tiles_count, game_background_tiles);                                // load
	set_bkg_tiles(0, 0, game_background_map_width, game_background_map_height, game_background_map);    // show

	// Load game symbols
	set_bkg_data(symbol_x_tiles_start, symbol_x_map_tiles, symbol_x_tiles);
	set_bkg_data(symbol_o_tiles_start, symbol_o_map_tiles, symbol_o_tiles);

    // Load grid cell selection cursor
    set_sprite_data(0, cursor_tiles_count, cursor_tiles);
	set_sprite_tile(0, 0);
    move_sprite(0, 80, 110);

	// --- Finish loading into VRAM and OAM ---
	DISPLAY_ON;
	enable_interrupts();
}