// common.h

#ifndef _COMMON_H
#define _COMMON_H

#define ARRAY_LEN(A)  sizeof(A) / sizeof(A[0])

// Controls board size and many other things
#define WORD_LENGTH 5
#define MAX_GUESSES 6

// Turns on some cheats to make testing easier
// #define DEBUG_ON

#define BOARD_HIGHLIGHT_NO  false
#define BOARD_HIGHLIGHT_YES true


extern uint8_t guess_num;
extern char guess[WORD_LENGTH+1];
extern char guesses[WORD_LENGTH+1][MAX_GUESSES];
extern char guessed_wrong[30];
extern char guessed_position[30];
extern char guessed_correct[30];
extern char word[WORD_LENGTH+1];

uint8_t contains(char *str, char c);

#endif