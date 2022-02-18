// gameplay.h

#ifndef _GAMEPLAY_H
#define _GAMEPLAY_H


#include <gbdk/platform.h>
#include <gbdk/incbin.h>
#include <gb/gbdecompress.h>

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <rand.h>

#include "decode.h"

#include "common.h"
#include "input.h"
#include "gfx_common.h"
#include "gameboy_color.h"

#include "keyboard.h"
#include "board.h"
#include "window.h"


void gameplay_init_answer_word(void);

void show_win_message(uint8_t guess_count);
void show_lose_message(char *correct_word);

void gameplay_handle_guess(void);

void gameplay_init_answer_word(void);
void gameplay_init(void);
void gameplay_restart(void);
void gameplay_run(void);

#endif
