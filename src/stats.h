// stats.h

#ifndef _STATS_H
#define _STATS_H

#define STATS_GAME_WON  true
#define STATS_GAME_LOST false


#define STATS_SIG_CHECK_0 0xA50Fu
#define STATS_SIG_CHECK_1 0x1E78u


// Struct containing the save game
// For MBC5 will be mapped to the SRAM through a pointer
typedef struct stats_rec {
    uint16_t save_check0;
    uint16_t save_check1;
    uint16_t games_count;
    uint16_t games_won;
    uint16_t games_lost;
    uint16_t games_streak;

    uint16_t guesses_min;
    uint16_t guesses_max;
    uint16_t guesses_sum;
} stats_rec;

void stats_reset(void);
void stats_update(bool game_won, uint8_t guess_count);
void stats_show(void);


#endif // _STATS_H


