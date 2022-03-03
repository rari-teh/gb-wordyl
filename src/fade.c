#include <gbdk/platform.h>

#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "fade.h"


const uint8_t fade_steps_dmg_normal[] = {
    DMG_PALETTE(DMG_WHITE,     DMG_LITE_GRAY, DMG_DARK_GRAY, DMG_BLACK),
    DMG_PALETTE(DMG_LITE_GRAY, DMG_DARK_GRAY, DMG_BLACK,     DMG_BLACK),
    DMG_PALETTE(DMG_DARK_GRAY, DMG_BLACK,     DMG_BLACK,     DMG_BLACK),
    DMG_PALETTE(DMG_BLACK,     DMG_BLACK,     DMG_BLACK,     DMG_BLACK)
};

const uint8_t fade_steps_dmg_invert[] = {
    DMG_PALETTE(DMG_BLACK,     DMG_DARK_GRAY, DMG_LITE_GRAY, DMG_WHITE),
    DMG_PALETTE(DMG_BLACK,     DMG_BLACK,     DMG_DARK_GRAY, DMG_LITE_GRAY),
    DMG_PALETTE(DMG_BLACK,     DMG_BLACK,     DMG_BLACK,     DMG_DARK_GRAY),
    DMG_PALETTE(DMG_BLACK,     DMG_BLACK,     DMG_BLACK,     DMG_BLACK)
};


const uint8_t * fade_steps_dmg = fade_steps_dmg_normal;


void fade_out(void) {
    for (uint8_t c = 0; c < ARRAY_LEN(fade_steps_dmg_normal); c++) {
        wait_vbl_done();
        BGP_REG = fade_steps_dmg[c];
        OBP0_REG = fade_steps_dmg[c];
        delay(70);
    }
}

void fade_in(void) {
    for (uint8_t c = 0; c < ARRAY_LEN(fade_steps_dmg_normal); c++) {
        wait_vbl_done();
        BGP_REG = fade_steps_dmg[ARRAY_LEN(fade_steps_dmg_normal) - 1 - c];
        OBP0_REG = fade_steps_dmg[ARRAY_LEN(fade_steps_dmg_normal) - 1 - c];
        delay(70);
    }
}
