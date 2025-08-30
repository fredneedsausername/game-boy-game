#include "separate_files.h"

void flip_cursor_visibility(void) {
    volatile static uint8_t frameCount = 0;
    volatile static uint8_t isCursorVisible = 1;

    // Flip cursor visibility
    if(++frameCount >= 30) {
        
        if(isCursorVisible & enableFlipCursorVisibility) {       // Hide cursor sprite
            isCursorVisible = 0;
            move_sprite(0, 0, 0);
        } else {                    // Show cursor sprite
            isCursorVisible = 1;
            move_sprite(0, 40 + cursorPosition.x * 40, 70 + cursorPosition.y * 40);
        }

        // Reset frame counter
        frameCount = 0;
    }
}