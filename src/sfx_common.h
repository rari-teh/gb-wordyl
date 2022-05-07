// sfx_common.h

#ifndef _SFX_COMMON_H
#define _SFX_COMMON_H

#include "cbtfx.h"
// Example sfx as placeholders
#include "sfx/SFX_00.h"
#include "sfx/SFX_01.h"
#include "sfx/SFX_02.h"
#include "sfx/SFX_03.h"
#include "sfx/SFX_04.h"
#include "sfx/SFX_05.h"
#include "sfx/SFX_06.h"
#include "sfx/SFX_07.h"
#include "sfx/SFX_08.h"
#include "sfx/SFX_09.h"


#define SFX_TILE_ADD           0x02
#define SFX_TILE_REMOVE        0x02
#define SFX_TILE_REVEAL_RESULT 0x02
#define SFX_CURSOR_MOVE        0x02
#define SFX_GAME_WON           0x02
#define SFX_GAME_LOST          0x02
#define SFX_GAME_FORFEIT       0x02 // maybe just same as SFX_GAME_LOST
#define SFX_EXIT_SPLASH        0x02
#define SFX_MENU_ACTION_ACKNOWLEDGE 0x02


extern const unsigned char * SFX_list[];

#define PLAY_SFX(SFX_NUM) CBTFX_init(SFX_list[(SFX_NUM)])

#endif