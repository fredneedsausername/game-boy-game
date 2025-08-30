#include "separate_files.h"

void accept_move(void) {

    uint8_t buttonPress;
    uint8_t movementAmount;
    int8_t xIncrement;
    int8_t yIncrement;
    uint8_t pixelScrollAmount;
    struct Coordinate_t newCursorPosition;
    int8_t xDifference;
    int8_t yDifference;
    int8_t xCounter;
    int8_t yCounter;
    int8_t xLimit;
    int8_t yLimit;

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

        if (movementAmount) {
            pixelScrollAmount = 40 * movementAmount;

            // Move cursor
            scroll_sprite(0, pixelScrollAmount * xIncrement, pixelScrollAmount * yIncrement);

        } else { // if that direction is blocked, look for potential diagonal jumps

            if(
                ((int8_t) (xIncrement + cursorPosition.x) == -1) | ((int8_t) (xIncrement + cursorPosition.x) == 3) |
                ((int8_t) (yIncrement + cursorPosition.y) == -1) | ((int8_t) (yIncrement + cursorPosition.y) == 3)
            ) {
                continue;
            }

            // If the direction is horizontal
            if (xIncrement) {

                if (cursorPosition.y) {
                    yIncrement = -1;
                    yCounter = 2;
                    yLimit = -1;
                } else {
                    yIncrement = 1;
                    yCounter = 0;
                    yLimit = 3;
                }

                for(int8_t i = cursorPosition.x + xIncrement; ((i != -1) & (i != 3)); i += xIncrement) {
                    for(int8_t j = yCounter; j != yLimit; j += yIncrement) {
                        if(gameGrid[i][j] == EMPTY_CELL_SYMBOL) {
                            newCursorPosition.x = i;
                            newCursorPosition.y = j;
                            i = -1 - xIncrement; // Exit outer loop
                            break;
                        }
                    }
                    // Be careful to add code here: if we just exited the inner loop and the if condition was true then 'i' is sure to kick us out of the loop after this 
                }

                yCounter = 0;
                yIncrement = 0;
                yLimit = 0;

            }

            // If the direction is vertical
            if (yIncrement) {

                if (cursorPosition.x) {
                    xIncrement = -1;
                    xCounter = 2;
                    xLimit = -1;
                } else {
                    xIncrement = 1;
                    xCounter = 0;
                    xLimit = 3;
                }

                for(int8_t i = cursorPosition.y + yIncrement; ((i != -1) & (i != 3)); i += yIncrement) {
                    for(int8_t j = xCounter; j != xLimit; j += xIncrement) {
                        if(gameGrid[j][i] == EMPTY_CELL_SYMBOL) {
                            newCursorPosition.x = j;
                            newCursorPosition.y = i;
                            i = -1 - yIncrement; // Exit outer loop
                            break;
                        }
                    }
                    // Be careful to add code here: if we just exited the inner loop and the if condition was true then 'i' is sure to kick us out of the loop after this 
                }

                xCounter = 0;
                xIncrement = 0;
                xLimit = 0;

            }

            xDifference = newCursorPosition.x - cursorPosition.x;

            // If the new cursor has moved at all, then all coordinates must have changed by at least one unit. Explaining why would take longer than for you to think about it.
            if (xDifference) { // if the cursor has moved at all

                yDifference = newCursorPosition.y - cursorPosition.y;

                // Move the cursor
                cursorPosition = newCursorPosition;
                scroll_sprite(0, 40 * xDifference, 40 * yDifference);

            } else { // If the cursor hasn't moved
                continue;
            }
        }
        
    }
}