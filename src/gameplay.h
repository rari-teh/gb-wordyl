// gameplay.h

#ifndef _GAMEPLAY_H
#define _GAMEPLAY_H

void gameplay_init_answer_word(void);

void show_win_message(uint8_t guess_count);
void show_lose_message(char *correct_word);

void gameplay_handle_guess(void);

// void gameplay_init_turn_gfx_on(void);
void gameplay_init_maps(void);
void gameplay_restart(void);
void gameplay_run(void);

#endif
