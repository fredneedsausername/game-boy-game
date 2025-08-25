#include "separate_files.h"

#define X_WON_BITMASK                   0b10000000U
#define O_WON_BITMASK                   0b01000000U
#define ASCENDING_DIAGONAL_BITMASK      0b00001000U
#define DESCENDING_DIAGONAL_BITMASK     0b00000100U


/*

enum GridCellSymbol_t {
    EMPTY_CELL_SYMBOL,
    X_CELL_SYMBOL,
    O_CELL_SYMBOL
};

enum GridCellSymbol_t gameGrid[3][3] = {
    {EMPTY_CELL_SYMBOL, EMPTY_CELL_SYMBOL, EMPTY_CELL_SYMBOL},
    {EMPTY_CELL_SYMBOL, EMPTY_CELL_SYMBOL, EMPTY_CELL_SYMBOL},
    {EMPTY_CELL_SYMBOL, EMPTY_CELL_SYMBOL, EMPTY_CELL_SYMBOL}
};

*/

void handle_win(void) {
    /*
    bits 0-1 -> number of horizontal/vertical row of win. If not horizontal or vertical, then 0
    bits 2-7 -> bitmasks defined at start of file
    */
    uint8_t winType = 0;

    // Check horizontal wins
    for(uint8_t i = 0; i != 3; i++) {
        if(((gameGrid[i][0]) == (gameGrid[i][1])) & ((gameGrid[i][1]) == (gameGrid[i][2]))) {
            switch(gameGrid[i][0]) {
                case X_CELL_SYMBOL:
                    winType |= X_WON_BITMASK;   // Register that if was X that won
                    winType |= i;               // Register which row of X was the one that won
                    break;
                case O_CELL_SYMBOL:
                    winType |= O_WON_BITMASK;   // Register that if was O that won
                    winType |= i;               // Register which column of O was the one that won
                    break;
                case EMPTY_CELL_SYMBOL:
                    break;
            }
            break;  // This may seem useless, but it is to protect the future programmer if they want to add code after the switch.            
        }
    }

    // Check vertical wins
    for(uint8_t i = 0; i != 3; i++) {
        if(((gameGrid[0][i]) == (gameGrid[1][i])) & ((gameGrid[1][i]) == (gameGrid[2][i]))) {
            switch(gameGrid[0][i]) {
                case X_CELL_SYMBOL:
                    winType |= X_WON_BITMASK;   // Register that if was X that won
                    winType |= i;               // Register which row of X was the one that won
                    break;
                case O_CELL_SYMBOL:
                    winType |= O_WON_BITMASK;   // Register that if was O that won
                    winType |= i;               // Register which column of O was the one that won
                    break;
                case EMPTY_CELL_SYMBOL:
                    break;
            }
            break;  // This may seem useless, but it is to protect the future programmer if they want to add code after the switch.            
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
                break;
        }
    }

    // Check ascending diagonal win
    if((gameGrid[0][2] == gameGrid[1][1]) & (gameGrid[1][1] == gameGrid[2][0])) {
        switch(gameGrid[0][0]) {
            case X_CELL_SYMBOL:
                winType |= X_WON_BITMASK;               // Register that if was X that won
                winType |= ASCENDING_DIAGONAL_BITMASK;  // Register that an ascending diagonal won
                break;
            case O_CELL_SYMBOL:
                winType |= O_WON_BITMASK;               // Register that if was O that won
                winType |= ASCENDING_DIAGONAL_BITMASK;  // Register that an ascending diagonal won
                break;
            case EMPTY_CELL_SYMBOL:
                break;
        }
    }

    if (winType) { // If there was any win
        enum GridCellSymbol_t drawSymbol; // Symbol to draw the line with

        // Determine win drawing symbol
        if(winType & X_WON_BITMASK) {
            drawSymbol = X_CELL_SYMBOL;
        } else if (winType & O_WON_BITMASK) {
            drawSymbol = O_CELL_SYMBOL;
        }

        if(winType & DESCENDING_DIAGONAL_BITMASK) {

        } else if (winType & ASCENDING_DIAGONAL_BITMASK) {

        } else {
            uint8_t drawLinePosition = (winType & 0b11); // The first two bytes have the information we want here
            
        }
    } else {
        uint8_t symbolCount = 0; // To check for tie

        for(uint8_t i = 0; i != 3; i++) {   // Count non-empty cells
            for (uint8_t j = 0; j != 3; j++) {
                if(gameGrid[i][j] != EMPTY_CELL_SYMBOL) {
                    symbolCount++;
                }
            }
        }

        if (symbolCount == 9) { // If there are no empty cells
            // Handle Tie
        }
    }
}