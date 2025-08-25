#include "separate_files.h"

void accept_move(void) {

    uint8_t buttonPress;
    uint8_t movementAmount;
    int8_t xIncrement;
    int8_t yIncrement;
    uint8_t pixelScrollAmount;

    while(1) {
        do {
            // Read pad
            buttonPress = waitpad(
                J_RIGHT     |
                J_LEFT      |
                J_UP        |
                J_DOWN      |
                J_A         |
                J_B
            );

            // Wait for complete button press
            waitpadup();

            // If one of the placement buttons is pressed at the same time as the movement buttons cancel the press
        } while ((buttonPress & (J_A | J_B)) && (buttonPress & (J_UP | J_DOWN | J_LEFT | J_RIGHT)));

        if (buttonPress & (J_A | J_B)) { // If A or B are pressed
            if(gameGrid[cursorPosition.x][cursorPosition.y] == EMPTY_CELL_SYMBOL) { // If the cell is free
                return; // Select move
            }
            buttonPress &= ~(J_A | J_B); // Register that the buttons have been processed
        }

        // If left and right are pressed at the same time, cancel the horizontal direction press
        if ((buttonPress & (J_LEFT | J_RIGHT)) == (J_LEFT | J_RIGHT)) {
            buttonPress &= ~(J_LEFT | J_RIGHT);
        }

        // If up and down are pressed at the same time, cancel the vertical direction press
        if ((buttonPress & (J_UP | J_DOWN)) == (J_UP | J_DOWN)) {
            buttonPress &= ~(J_UP | J_DOWN);
        }

        // Just consider one direction of movement for simplicity, prioriting vertical movement
        if ((buttonPress & (J_UP | J_DOWN)) && (buttonPress & (J_LEFT & J_RIGHT))) {
            buttonPress &= ~(J_LEFT | J_RIGHT);
        }

        // Find the first free square in the desired direction
        xIncrement = 0;
        yIncrement = 0;

        // Only one direction can be considered at a time, because of the earlier filters
        // Set the increments to check for empty squares
        switch(buttonPress) {
            case J_UP:
                yIncrement = -1;
                break;
            case J_DOWN:
                yIncrement = 1;
                break;
            case J_LEFT:
                xIncrement = -1;
                break;
            case J_RIGHT:
                xIncrement = 1;
                break;
        }

        movementAmount = 0;

        if(xIncrement) {
            // Check for free squares horizontally
            for(int8_t i = cursorPosition.x; (i != (-1)) && (i != 3); i += xIncrement) {

                // Cursor at left wall cannot go left
                if(i + xIncrement == -1) {
                    movementAmount = 0;
                    break;
                }

                // Cursor at right wall cannot go right
                if(i + xIncrement == 3) {
                    movementAmount = 0;
                    break;
                }

                // Increment movement to update sprite
                movementAmount++;

                // If a free cell is found set the cursor position to it
                if (gameGrid[i + xIncrement][cursorPosition.y] == EMPTY_CELL_SYMBOL) {
                    cursorPosition.x = i + xIncrement;
                    break;
                }
            }
        }

        if(yIncrement) {
            // Check for free squares vertically
            for(int8_t i = cursorPosition.y; (i != (-1)) && (i != 3); i += yIncrement) {

                // Cursor at upper wall cannot go up
                if(i + yIncrement == -1) {
                    movementAmount = 0;
                    break;
                }

                // Cursor at bottom wall cannot go down
                if(i + yIncrement == 3) {
                    movementAmount = 0;
                    break;
                }

                // Increment movement to update sprite
                movementAmount++;

                // If a free cell is found set the cursor position to it
                if (gameGrid[cursorPosition.x][i + yIncrement] == EMPTY_CELL_SYMBOL) {
                    cursorPosition.y = i + yIncrement;
                    break;
                }
            }
        }

        pixelScrollAmount = 40 * movementAmount;

        scroll_sprite(0, pixelScrollAmount * xIncrement, pixelScrollAmount * yIncrement);
    }
}