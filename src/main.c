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