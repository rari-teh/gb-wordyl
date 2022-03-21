// common.h

#ifndef _COMMON_H
#define _COMMON_H

#include <stdbool.h>

// #define _DO_EXPAND(VAL)  VAL ## 1
// #define EXPAND(VAL)     _DO_EXPAND(VAL)

#define _STR(s) #s
#define STR(s) _STR(s)

#define ARRAY_LEN(A)  sizeof(A) / sizeof(A[0])


// Game flow control
#define GAME_STATE_INTRO      0
#define GAME_STATE_FIRSTSTART 1
#define GAME_STATE_RESTART    2
#define GAME_STATE_RUNNING    3
#define GAME_STATE_OVER       4


// Controls board size and many other things
#define WORD_LENGTH 5
#define MAX_GUESSES 6

#define LETTER_CURSOR_START 0
#define LETTER_CURSOR_MAX   (WORD_LENGTH - 1)

// Cheats for testing
// #define DEBUG_REVEAL_WHILE_TYPE
// #define DEBUG_FORCE_WORD "AMPLE"
// #define DEBUG_FORCE_WORD "HUMAN"
// #define DEBUG_REVEAL_WORD
// #define DEBUG_FORCE_WORD_BY_NUM  2200u // 2200u // 2295u // 2200u // 0u // 100u // 1351u
// TODO: show answer word number

#define BOARD_HIGHLIGHT_NO  false
#define BOARD_HIGHLIGHT_YES true

// IMPORANT: Order of values is important and relied on for sorting, do not change!
// Used :
// * Printing color lookup table
// * Letter matching evaluation ranking
// * Ensuring keyboard highlighting only gets upgraded and not downgraded with progressive guesses
#define LETTER_RIGHT_PLACE 3u
#define LETTER_WRONG_PLACE 2u
#define LETTER_NOT_MATCHED 1u
#define LETTER_NOT_SET     0u

#define STATUS_GAME_WON      2u
#define STATUS_GAME_LOST     1u
#define STATUS_GAME_CONTINUE 0u

#define GUESS_LETTER_EMPTY   0

extern uint8_t game_state;

extern bool opt_hard_mode_enabled;
extern bool opt_autofill_enabled;

extern uint8_t guess_num;
extern uint8_t guess_letter_cursor;

extern uint8_t prev_guess_eval[WORD_LENGTH];
extern uint8_t guess_eval[WORD_LENGTH];

extern char exact_matches[WORD_LENGTH+1];
extern char prev_guess[WORD_LENGTH+1];
extern char guess[WORD_LENGTH+1];
extern char word[WORD_LENGTH+1];

uint8_t * str_u16_left_at_X(uint8_t * p_str, uint16_t num);

void copy_or_reset_prev_guess(char* guess);

// uint8_t contains(char *str, char c); // TODO: DEAD CODE
void evaluate_letters(char * guess);
bool evaluate_guess_hard_mode(char * guess);

#endif