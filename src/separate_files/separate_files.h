// Every function gets a separate file because sdcc supports only per-file recompilation, and not per-function recompilation

#include <gb/gb.h>
#include <rand.h>
#include <stdbool.h>
#include "assets/cursor_tiles.h"
#include "assets/game_background_map.h"
#include "assets/game_background_tiles.h"
#include "assets/symbol_o_map.h"
#include "assets/symbol_o_tiles.h"
#include "assets/symbol_x_map.h"
#include "assets/symbol_x_tiles.h"
#include "assets/empty_square_map.h"
#include "assets/empty_square_tiles.h"

#define symbol_x_tiles_start 56
#define symbol_o_tiles_start 65
#define empty_square_start 74

// --- STRUCTS ---

struct Coordinate_t {
    int8_t x;
    int8_t y;
};

// --- ENUMS ---

enum GridCellSymbol_t {
    EMPTY_CELL_SYMBOL,
    X_CELL_SYMBOL,
    O_CELL_SYMBOL
};

// --- GLOBALS ---

extern enum GridCellSymbol_t currentTurn;
extern enum GridCellSymbol_t gameGrid[3][3];
extern struct Coordinate_t cursorPosition;
extern bool enableFlipCursorVisibility;

// --- FUNCTIONS ---

void init_game(void);
void init_vram_oam(void);
uint8_t fred_arand(uint8_t roof);                // Generates random number from 0 to roof, both included
void init_fred_arand(uint16_t arand_seed);
void pass_turn(void);
void game_loop(void);
void accept_move(void);      // Returns only when the player has inserted a valid move, and the move is in the current cursor position
void flip_cursor_visibility(void); // VBL interrupt handler
void init_interrupts(void);
void place_symbol(void); // takes cursor position information and symbol to place (current turn) from global variables. Make sure to vsync first!
void draw_symbol(uint8_t xTileCoordinate, uint8_t yTileCoordinate, enum GridCellSymbol_t symbol);
void handle_win(void);