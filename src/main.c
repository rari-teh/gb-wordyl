/* Wordle clone for Game Boy.
 * A weekend project by stacksmashing.
 * Contact: twitter.com/ghidraninja
 *          code@stacksmashing.net
 *
 * Dictionary improvements by zeta_two
 * Further dictionary improvements and highlighting fixes by arpruss
 * Major rework by bbbbbr
 */

#include <gbdk/platform.h>
#include <gbdk/incbin.h>
#include <stdint.h>
#include <stdbool.h>

#include "common.h"
#include "input.h"
#include "gfx_common.h"
#include "gameboy_color.h"
#include "fade.h"

#include "intro_splash.h"
#include "board.h"
#include "gameplay.h"
#include "stats.h"


void main() {

    fade_out();
    SHOW_WIN;
    SHOW_BKG;
    SHOW_SPRITES;

    cgb_check_and_init();
    gfx_load();
    stats_reset();
    game_state = GAME_STATE_INTRO;

    while(1) {
        switch (game_state) {

            case GAME_STATE_INTRO:
                splash_run();
                // screen now: faded out
                BOARD_SET_LAYOUT_GAME;
                gameplay_init_maps();
                game_state = GAME_STATE_RESTART;
                break;

            case GAME_STATE_RESTART:
                gameplay_restart();
                fade_in();
                game_state = GAME_STATE_RUNNING;
                break;

            case GAME_STATE_RUNNING:
                gameplay_run();
                break;

            case GAME_STATE_OVER:
                stats_show();
                fade_out();
                game_state = GAME_STATE_RESTART;
                break;
        }
    }
}
