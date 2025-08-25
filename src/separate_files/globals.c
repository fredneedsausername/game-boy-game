#include "separate_files.h"

enum GridCellSymbol_t currentTurn = O_CELL_SYMBOL; // It will immediately be switched to be X

struct Coordinate_t cursorPosition = {.x=1, .y=1};

enum GridCellSymbol_t gameGrid[3][3] = {
    {EMPTY_CELL_SYMBOL, EMPTY_CELL_SYMBOL, EMPTY_CELL_SYMBOL},
    {EMPTY_CELL_SYMBOL, EMPTY_CELL_SYMBOL, EMPTY_CELL_SYMBOL},
    {EMPTY_CELL_SYMBOL, EMPTY_CELL_SYMBOL, EMPTY_CELL_SYMBOL}
};