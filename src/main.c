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

#include "gameplay.h"
#include "stats.h"

void main() {

    DISPLAY_OFF;
    cgb_check_and_init();
    stats_reset();
    game_state = GAME_STATE_INIT;

    while(1) {
        switch (game_state) {
            case GAME_STATE_INIT:
                gameplay_init();
                game_state = GAME_STATE_RESTART;
                break;

            case GAME_STATE_RESTART:
                gameplay_restart();
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
