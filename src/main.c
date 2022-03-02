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

#include "intro_splash.h"
#include "board.h"
#include "gameplay.h"
#include "stats.h"

void main() {

    DISPLAY_OFF;
    cgb_check_and_init();
    stats_reset();
    game_state = GAME_STATE_INTRO;

    while(1) {
        switch (game_state) {
            case GAME_STATE_INTRO:
// FADE OUT
                board_initgfx(); // TODO: move out of main loop
                BOARD_SET_LAYOUT_SPLASH;
                splash_init_maps();
                gameplay_init_turn_gfx_on();  // TODO: move out of main loop
// FADE IN
                splash_animate_title();
                waitpadticked_lowcpu(J_ANY_KEY, NULL);
// FADE OUT
                game_state = GAME_STATE_FIRSTSTART;
                break;

            case GAME_STATE_FIRSTSTART:
                BOARD_SET_LAYOUT_GAME;
// FIXME: seeing artifacting here leftover from splash - fade should fix it
                gameplay_init_maps();
                game_state = GAME_STATE_RESTART;
                break;

            case GAME_STATE_RESTART:
// FADE OUT
                gameplay_restart();
// FADE IN
                game_state = GAME_STATE_RUNNING;
                break;

            case GAME_STATE_RUNNING:
                gameplay_run();
                break;

            case GAME_STATE_OVER:
                stats_show();
                game_state = GAME_STATE_RESTART;
                break;
        }
    }
}
