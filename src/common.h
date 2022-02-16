// common.h

#ifndef _COMMON_H
#define _COMMON_H

#define ARRAY_LEN(A)  sizeof(A) / sizeof(A[0])

// Controls board size and many other things
#define WORD_LENGTH 5
#define MAX_GUESSES 6

// Cheats for testing
// #define DEBUG_REVEAL_WHILE_TYPE
// #define DEBUG_FORCE_WORD "AMPLE"
// #define DEBUG_REVEAL_WORD

#define BOARD_HIGHLIGHT_NO  false
#define BOARD_HIGHLIGHT_YES true

// Order of values is important and relied on for sorting, do not change
#define LETTER_RIGHT_PLACE 3u
#define LETTER_WRONG_PLACE 2u
#define LETTER_NOT_MATCHED 1u
#define LETTER_NOT_SET     0u

#define STATUS_GAME_WON      2u
#define STATUS_GAME_LOST     1u
#define STATUS_GAME_CONTINUE 0u

#define GUESS_LETTER_EMPTY   0


extern uint8_t guess_eval[5];

extern uint8_t guess_num;

extern char guess[WORD_LENGTH+1];
extern char word[WORD_LENGTH+1];

// TODO: DEAD CODE?
// extern char guesses[WORD_LENGTH+1][MAX_GUESSES];

// TODO: DEAD CODE?
// extern char guessed_wrong[30];
// extern char guessed_position[30];
// extern char guessed_correct[30];

// uint8_t contains(char *str, char c); // TODO: DEAD CODE
void evaluate_letters(char* guess);

#endif