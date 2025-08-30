#include "separate_files.h"

#define X_WON_BITMASK                   0b10000000U
#define O_WON_BITMASK                   0b01000000U
#define HORIZONTAL_WIN_BITMASK          0b00100000U
#define VERTICAL_WIN_BITMASK            0b00010000U
#define ASCENDING_DIAGONAL_BITMASK      0b00001000U
#define DESCENDING_DIAGONAL_BITMASK     0b00000100U

// Flashes symbols and clears the tiles of those symbols
static void flash_clear_symbols(struct Coordinate_t* tiles, enum GridCellSymbol_t* symbols, uint8_t nTiles) {

    // Make the cursor not flash during the win animation
    enableFlipCursorVisibility = false;

    uint8_t flashQuantity = 3;

    // Flash the symbols and clear the screen space for them
    for (uint8_t i = 0; i != flashQuantity; i++) {

        for (uint8_t j = 0; j != nTiles; j++) {
            draw_symbol(1 + 5 * tiles[j].x, 4 + 5 * tiles[j].y, symbols[j]);
        }

        delay(100);

        for (uint8_t j = 0; j != nTiles; j++) {
            draw_symbol(1 + 5 * tiles[j].x, 4 + 5 * tiles[j].y, EMPTY_CELL_SYMBOL);
        }

    }

    // Update game data, now it considers those squares as cleared
    for (uint8_t i = 0; i != nTiles; i++) {
        gameGrid[tiles[i].x][tiles[i].y] = EMPTY_CELL_SYMBOL;
    }

    // Enable the cursor flash after the win animation
    enableFlipCursorVisibility = true;
}

void handle_win(void) {
    /*
    bits 0-1 -> number of horizontal/vertical row of win. If not horizontal or vertical, then 0
    bits 2-7 -> bitmasks defined at start of file
    */
    uint8_t winType = 0;
    bool symbolFound = false;

    // Check horizontal wins
    for(uint8_t i = 0; (i != 3) & !symbolFound; i++) {
        if(((gameGrid[0][i]) == (gameGrid[1][i])) & ((gameGrid[1][i]) == (gameGrid[2][i]))) {
            switch(gameGrid[0][i]) {
                case X_CELL_SYMBOL:
                    winType |= X_WON_BITMASK;           // Register that if was X that won
                    winType |= HORIZONTAL_WIN_BITMASK;  // Register that it was a horizontal win
                    winType |= i;                       // Register which row of X was the one that won
                    symbolFound = true;
                    break;
                case O_CELL_SYMBOL:
                    winType |= O_WON_BITMASK;           // Register that if was O that won
                    winType |= HORIZONTAL_WIN_BITMASK;  // Register that it was a horizontal win
                    winType |= i;                       // Register which column of O was the one that won
                    symbolFound = true;
                    break;
                case EMPTY_CELL_SYMBOL:
                    // Keep this empty or modify with caution. If you set winType to anything without setting its flags in exactly the values that were planned, the subsequent "if (winType) {" statement will have logical bugs
                    break;
            }
        }
    }

    // Check vertical wins
    for(uint8_t i = 0; (i != 3) & !symbolFound; i++) {
        if(((gameGrid[i][0]) == (gameGrid[i][1])) & ((gameGrid[i][1]) == (gameGrid[i][2]))) {
            switch(gameGrid[i][0]) {
                case X_CELL_SYMBOL:
                    winType |= X_WON_BITMASK;           // Register that if was X that won
                    winType |= VERTICAL_WIN_BITMASK;    // Register that it was a vertical win
                    winType |= i;                       // Register which row of X was the one that won
                    symbolFound = true;
                    break;
                case O_CELL_SYMBOL:
                    winType |= O_WON_BITMASK;           // Register that if was O that won
                    winType |= VERTICAL_WIN_BITMASK;    // Register that it was a vertical win
                    winType |= i;                       // Register which column of O was the one that won
                    symbolFound = true;
                    break;
                case EMPTY_CELL_SYMBOL:
                    // Keep this empty or modify with caution. If you set winType to anything without setting its flags in exactly the values that were planned, the subsequent "if (winType) {" statement will have logical bugs
                    break;
            }
        }
    }
    
    // Check descending diagonal win
    if((gameGrid[0][0] == gameGrid[1][1]) & (gameGrid[1][1] == gameGrid[2][2])) {
        switch(gameGrid[0][0]) {
            case X_CELL_SYMBOL:
                winType |= X_WON_BITMASK;               // Register that if was X that won
                winType |= DESCENDING_DIAGONAL_BITMASK; // Register that a descending diagonal won
                break;
            case O_CELL_SYMBOL:
                winType |= O_WON_BITMASK;               // Register that if was O that won
                winType |= DESCENDING_DIAGONAL_BITMASK; // Register that a descending diagonal won
                break;
            case EMPTY_CELL_SYMBOL:
                // Keep this empty or modify with caution. If you set winType to anything without setting its flags in exactly the values that were planned, the subsequent "if (winType) {" statement will have logical bugs
                break;
        }
    }

    // Check ascending diagonal win
    if((gameGrid[0][2] == gameGrid[1][1]) & (gameGrid[1][1] == gameGrid[2][0])) {
        switch(gameGrid[0][2]) {
            case X_CELL_SYMBOL:
                winType |= X_WON_BITMASK;               // Register that if was X that won
                winType |= ASCENDING_DIAGONAL_BITMASK;  // Register that an ascending diagonal won
                break;
            case O_CELL_SYMBOL:
                winType |= O_WON_BITMASK;               // Register that if was O that won
                winType |= ASCENDING_DIAGONAL_BITMASK;  // Register that an ascending diagonal won
                break;
            case EMPTY_CELL_SYMBOL:
                // Keep this empty or modify with caution. If you set winType to anything without setting its flags in exactly the values that were planned, the subsequent "if (winType) {" statement will have logical bugs
                break;
        }
    }

    if (winType) { // If there was any win

        enum GridCellSymbol_t drawSymbol = EMPTY_CELL_SYMBOL; // Symbol to draw the line with

        // Determine win drawing symbol
        if(winType & X_WON_BITMASK) {
            drawSymbol = X_CELL_SYMBOL;
        } else if (winType & O_WON_BITMASK) {
            drawSymbol = O_CELL_SYMBOL;
        }

        struct Coordinate_t coords[3];
        const enum GridCellSymbol_t symbols[] = {drawSymbol, drawSymbol, drawSymbol};

        if(winType & DESCENDING_DIAGONAL_BITMASK) {

            const struct Coordinate_t coordsValues[] = { {.x=0, .y=0}, {.x=1, .y=1}, {.x=2, .y=2} };
            
            for(uint8_t i = 0; i != 3; i++) {
                coords[i] = coordsValues[i];
            }

        } else if (winType & ASCENDING_DIAGONAL_BITMASK) {

            const struct Coordinate_t coordsValues[] = { {.x=0, .y=2}, {.x=1, .y=1}, {.x=2, .y=0} };
            
            for(uint8_t i = 0; i != 3; i++) {
                coords[i] = coordsValues[i];
            }

        } else {
            uint8_t drawLinePosition = (winType & 0b11); // The first two bytes have the information we want here

            if(winType & HORIZONTAL_WIN_BITMASK) {

                const struct Coordinate_t coordsValues[] = { 
                    {.x=0, .y=drawLinePosition}, 
                    {.x=1, .y=drawLinePosition}, 
                    {.x=2, .y=drawLinePosition} 
                };
            
                for(uint8_t i = 0; i != 3; i++) {
                    coords[i] = coordsValues[i];
                }

            } else if (winType & VERTICAL_WIN_BITMASK) {
            
                const struct Coordinate_t coordsValues[] = { 
                    {.x=drawLinePosition, .y=0}, 
                    {.x=drawLinePosition, .y=1}, 
                    {.x=drawLinePosition, .y=2} 
                };
            
                for(uint8_t i = 0; i != 3; i++) {
                    coords[i] = coordsValues[i];
                }
            }
        }

        flash_clear_symbols(coords, symbols, 3);

    } else { // Maybe tie or nothing at all
        uint8_t symbolCount = 0; // To check for tie

        for(uint8_t i = 0; i != 3; i++) {   // Count non-empty cells
            for (uint8_t j = 0; j != 3; j++) {
                if(gameGrid[i][j] != EMPTY_CELL_SYMBOL) {
                    symbolCount++;
                }
            }
        }

        if (symbolCount == 9) { // If there are no empty cells

            struct Coordinate_t coords[9];
            enum GridCellSymbol_t symbols[9];

            // Get all coordinates
            uint8_t arrayIndex;
            for (uint8_t i = 0; i != 3; i++) {
                for (uint8_t j = 0; j != 3; j++) {
                    arrayIndex = j + i*3; 
                    coords[arrayIndex].x = j;
                    coords[arrayIndex].y = i;
                }
            }

            // Get all symbols
            for (uint8_t i = 0; i != 9; i++) {
                symbols[i] = gameGrid[coords[i].x][coords[i].y];
            }

            flash_clear_symbols(coords, symbols, 9);
        }
    }

    // No tie nor win.
    return;
}