// stats.c

#include <gbdk/platform.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "common.h"
#include "window.h"
#include <lang_text.h>


#define STATS_GUESSES_MAX

uint16_t stats_games_count;
uint16_t stats_games_won;
uint16_t stats_games_lost;
uint16_t stats_games_streak;

uint16_t stats_guesses_min;
uint16_t stats_guesses_max;
uint16_t stats_guesses_sum;

// Only really needed if 32K save files become a thing
void stats_reset(void) {

    stats_games_count = 0;
    stats_games_won = 0;
    stats_games_lost = 0;
    stats_games_streak = 0;

    stats_guesses_min = MAX_GUESSES;
    stats_guesses_max = 0;
    stats_guesses_sum = 0;
}


// Called after end of a game round and before stats_show
void stats_update(bool game_won, uint8_t guess_num) {

    stats_games_count++;

    if (game_won) {
        stats_games_won++;
        stats_games_streak++;

        if (guess_num < stats_guesses_min)
            stats_guesses_min = guess_num;
        if (guess_num > stats_guesses_max)
            stats_guesses_max = guess_num;

        stats_guesses_sum += guess_num;
    }
    else {
        stats_games_lost++;
        stats_games_streak = 0;
    }
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


    str_u16_left_at_X(stats_str, stats_games_count);
    str_u16_left_at_X(stats_str, stats_games_streak);
    str_u16_left_at_X(stats_str, stats_games_won);
    str_u16_left_at_X(stats_str, stats_games_lost);

    str_u16_left_at_X(stats_str, stats_guesses_min);
    str_u16_left_at_X(stats_str, stats_guesses_max);
    str_u16_left_at_X(stats_str, stats_guesses_sum / stats_games_won);

    win_dialog_show_message(STATS_DIALOG_WIN_Y, stats_str, NULL);
}

