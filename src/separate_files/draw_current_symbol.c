#include "separate_files.h"

#define symbol_height 3
#define symbol_width 3

void draw_current_symbol(uint8_t xTileCoordinate, uint8_t yTileCoordinate) {
    const uint8_t * symbolMap;
    uint8_t symbolStart;

    if(currentTurn == X_CELL_SYMBOL) {
        symbolMap = symbol_x_map;
        symbolStart = symbol_x_tiles_start;
    } else  {
        symbolMap = symbol_o_map;
        symbolStart = symbol_o_tiles_start;
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