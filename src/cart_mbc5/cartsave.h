#ifndef __CARTSAVE_H_INCLUDE
#define __CARTSAVE_H_INCLUDE

#include <string.h>
#include <stdint.h>

extern stats_rec __at (0xA000) sram_stats;

inline void cartsave_restore_data(uint8_t * dest, uint16_t size) {
    ENABLE_RAM_MBC5;
    memcpy(dest, &sram_stats, size);
    DISABLE_RAM_MBC5;
}


inline void cartsave_save_data(uint8_t * source, uint16_t size) {
    ENABLE_RAM_MBC5;
    memcpy(&sram_stats, source, size);
    DISABLE_RAM_MBC5;
}

#endif
