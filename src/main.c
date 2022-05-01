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
#include <rand.h>

#include "common.h"
#include "settings.h"
#include "input.h"
#include "gfx_common.h"
#include "gameboy_color.h"
#include "fade.h"

#include "intro_splash.h"
#include "board.h"
#include "gameplay.h"
#include "stats.h"

#ifdef TEST_DICT_ENABLED
    #include "decode.h"
#endif


bool is_first_run = true;

fixed rand_seed = {.w = 0x0000u};


void main() {

    fade_out();
    SHOW_WIN;
    SHOW_BKG;

    cgb_check_and_init();
    gfx_load();
    game_state = GAME_STATE_INTRO;

    while(1) {
        switch (game_state) {

            case GAME_STATE_INTRO:
                splash_run();
                // First half of random init (button press to exit splash)
                rand_seed.h = DIV_REG;
                // screen now: faded out
                BOARD_SET_LAYOUT_GAME;
                gameplay_init_maps();
                game_state = GAME_STATE_RESTART;
                break;

            case GAME_STATE_RESTART:
                gameplay_restart();
                fade_in();
                if (is_first_run) {
                    show_intro_message();
                    is_first_run = false;
                    // Second half of random init (after button press to exit welcome dialog)
                    rand_seed.l = DIV_REG;
                    initrand(rand_seed.w);

                    // Wait to turn on all sprites until after first popup intro message
                    SHOW_SPRITES;

                    // Load or reset stats depending on cart type
                    settings_load();
                    #ifdef TEST_DICT_ENABLED
                        dumpTestToEmuConsole();
                    #endif
                }
                gameplay_init_answer_word();
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
