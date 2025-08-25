#include "separate_files.h"

void init_interrupts(void) {
    add_VBL(flip_cursor_visibility);
}