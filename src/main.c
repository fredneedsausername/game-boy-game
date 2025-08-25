#include "separate_files/separate_files.h"

void init_everything(void) {
	init_game();
	init_vram_oam();
	init_interrupts();
	pass_turn();
}

void main(void) {
    init_everything();
	game_loop();
}
	/*

    vsync();								// Done processing, yield CPU and wait for start of next frame

    delay();
	
	move_sprite()

	waitpad()
	waitpadup()

	set_sprite_data()						// set_sprite_data loads the sprite graphics into memory (like loading a sprite sheet)
	set_sprite_tile()						// set_sprite_tile arranges a sprite on the screen (like placing pieces on a board)

    init everything, then go into a game loop
	HAVE MUSIC IN THE GAME

	How long does V-Blank last?
	Let’s do the numbers:
		- The Game Boy runs at ~59.7 Hz (1 frame ≈ 16.74 ms).
		- Each frame is 154 scanlines.
			o 144 visible (drawing screen).
			o 10 lines of V-Blank.
		- Each scanline = 456 CPU cycles.
		- So V-Blank = 10 × 456 = 4560 CPU cycles.

	In time:
		- Game Boy CPU = 4.19 MHz.
		- 4560 cycles ≈ 1.1 ms of real time per frame.

	---

	Tic tac toe:
	- X starts
	- X puts their X in a square
	- Check if anybody won
	- O starts
	- O puts their O in a square
	---
	- When checking if anybody won, if won/stalemate:
		- Show win/stalemate
		- block game in that screen
	*/

