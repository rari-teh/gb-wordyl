#include <gbdk/platform.h>
#include <stdint.h>
#include <string.h>

#include "../common.h"


extern void flash_sector_address;
extern uint8_t erase_flash() OLDCALL;                                 // erase FLASH sector
extern uint8_t write_flash(uint8_t * source, uint16_t size) OLDCALL;  // write data to FLASH


void cartsave_restore_data(void) {

    // Copy from flash into game settings
    memcpy((void *)&game_settings, &flash_sector_address, sizeof(game_settings));
}


// TODO: warning on failure to save?
void cartsave_save_data(void) {

    if (!erase_flash()) return 0;
    return write_flash((void *)&game_settings, sizeof(game_settings));
}
