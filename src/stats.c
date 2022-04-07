// stats.c

#include <gbdk/platform.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "common.h"
#include "window.h"
#include <lang_text.h>

#include "stats.h"


#if defined(CART_31k_1kflash) || defined(CART_mbc5)
    #include <cartsave.h>
#endif


stats_rec stats;



// Zeros out stats and if supported saves the change
void stats_reset(void) {

    stats.save_check0 = STATS_SIG_CHECK_0;
    stats.save_check1 = STATS_SIG_CHECK_1;

    stats.games_count = 0;
    stats.games_won = 0;
    stats.games_lost = 0;
    stats.games_streak = 0;

    stats.guesses_min = MAX_GUESSES;
    stats.guesses_max = 0;
    stats.guesses_sum = 0;

    // For relevant carts, save the reset stats
    #if defined(CART_31k_1kflash) || defined(CART_mbc5)
        cartsave_save_data((void *)&stats, sizeof(stats));
    #endif
}


void stats_load(void) {

    #if defined(CART_31k_1kflash) || defined(CART_mbc5)
        // Check signature, reset stats and notify if signature failed
        // It is expected to fail on first power-up
        cartsave_restore_data((void *)&stats, sizeof(stats));

        if ((stats.save_check0 != STATS_SIG_CHECK_0) || (stats.save_check1 != STATS_SIG_CHECK_1)) {
            stats_reset();
            win_dialog_show_message(STATS_RESET_DIALOG_WIN_Y, __MESSAGE_STATS_RESET_STR ,NULL);
        }
    #else
        // (bare 32k) Saveless cart just resets stats instead of loading
        stats_reset();
    #endif
}


// Called after end of a game round and before stats_show
void stats_update(bool game_won, uint8_t guess_num) {

    stats.games_count++;

    if (game_won) {
        stats.games_won++;
        stats.games_streak++;

        if (guess_num < stats.guesses_min)
            stats.guesses_min = guess_num;
        if (guess_num > stats.guesses_max)
            stats.guesses_max = guess_num;

        stats.guesses_sum += guess_num;
    }
    else {
        stats.games_lost++;
        stats.games_streak = 0;
    }

    // For relevant carts, save the reset stats
    #if defined(CART_31k_1kflash) || defined(CART_mbc5)
        cartsave_save_data((void *)&stats, sizeof(stats));
    #endif
}


const uint8_t stats_str_template[] = __STATS_TEMPLATE_STR;
uint8_t stats_str[sizeof(stats_str_template)];

// Display current stats in a popup window at end of a game round
void stats_show(void) {

    // Reset display template
    // memcpy(stats_str, stats_str_template, sizeof(stats_str_template));
    // About 1/2 the size of including memcpy()
    uint8_t * p_dest = stats_str;
    const uint8_t * p_src = stats_str_template;
    while (p_src != stats_str_template + sizeof(stats_str_template)) {
        *p_dest++ = *p_src++;
    }


    str_u16_left_at_X(stats_str, stats.games_count);
    str_u16_left_at_X(stats_str, stats.games_streak);
    str_u16_left_at_X(stats_str, stats.games_won);
    str_u16_left_at_X(stats_str, stats.games_lost);

    str_u16_left_at_X(stats_str, stats.guesses_min);
    str_u16_left_at_X(stats_str, stats.guesses_max);
    str_u16_left_at_X(stats_str, stats.guesses_sum / stats.games_won);

    win_dialog_show_message(STATS_DIALOG_WIN_Y, stats_str, NULL);
}

