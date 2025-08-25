#include "separate_files.h"

void game_loop(void) {
	while(1) {
		accept_move();
		place_symbol();
		pass_turn();
		// detect win
		// have a mode to play against the computer and one to play in two players
		// make the game slowly and animatedly draw a line when it detects that a player has won, through the three symbols that were put in a row.
	}
}