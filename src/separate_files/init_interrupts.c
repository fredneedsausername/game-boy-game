#include "separate_files.h"

void init_interrupts(void) {
    enableFlipCursorVisibility = true;
    add_VBL(flip_cursor_visibility);
}