#include "separate_files.h"

uint8_t fred_arand(uint8_t roof) {
    return( arand()%(roof+1) );
}

void init_fred_arand(uint16_t arand_seed) {
    initarand((uint16_t) arand_seed);
}