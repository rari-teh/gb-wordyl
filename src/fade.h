// fade.h

#ifndef _FADE_H
#define _FADE_H

#include "common.h"

extern const uint8_t fade_steps_dmg_normal[];
extern const uint8_t fade_steps_dmg_invert[];
extern const uint8_t * fade_steps_dmg;


void fade_in(void);
void fade_out(void);

#define DMG_PAL_INVERT_ON  fade_steps_dmg = fade_steps_dmg_invert
#define DMG_PAL_INVERT_OFF fade_steps_dmg = fade_steps_dmg_normal

#endif
