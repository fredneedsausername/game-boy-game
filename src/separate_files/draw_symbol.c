#include "separate_files.h"

#define symbol_height 3
#define symbol_width 3

void draw_symbol(uint8_t xTileCoordinate, uint8_t yTileCoordinate, enum GridCellSymbol_t symbol) {
    const uint8_t * symbolMap;
    uint8_t symbolStart;

    switch (symbol) {
        case X_CELL_SYMBOL:
            symbolMap = symbol_x_map;
            symbolStart = symbol_x_tiles_start;
            break;
        case O_CELL_SYMBOL:
            symbolMap = symbol_o_map;
            symbolStart = symbol_o_tiles_start;
            break;
        case EMPTY_CELL_SYMBOL:
            symbolMap = empty_square_map;
            symbolStart = empty_square_start;
            break;
    }

    // Draw symbol
    vsync();
    set_bkg_based_tiles(
        xTileCoordinate,
        yTileCoordinate,
        symbol_height,
        symbol_width,
        symbolMap,
        symbolStart
    );
}