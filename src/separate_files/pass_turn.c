#include "separate_files.h"

void pass_turn(void) {
    if (currentTurn == X_CELL_SYMBOL) {
        currentTurn = O_CELL_SYMBOL;
    } else {
        currentTurn = X_CELL_SYMBOL;
    }

    // Update "turn" text
    draw_symbol(16, 8, currentTurn);
}