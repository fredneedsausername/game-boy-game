#include "separate_files.h"

void place_symbol(void) {

    // Update game data
    gameGrid[cursorPosition.x][cursorPosition.y] = currentTurn;

    draw_symbol(1 + 5 * cursorPosition.x, 4 + 5 * cursorPosition.y, currentTurn);
}