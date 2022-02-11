// common.c

#include <gbdk/platform.h>
#include <string.h>
#include <stdint.h>

#include "common.h"

uint8_t guess_num;
char guess[WORD_LENGTH+1];
char guesses[WORD_LENGTH+1][MAX_GUESSES];
char guessed_wrong[30];
char guessed_position[30];
char guessed_correct[30];
char word[WORD_LENGTH+1];


// Ceck if a letter is in a string
uint8_t contains(char *str, char c) {

    uint8_t l = strlen(str);

    for (uint8_t i=0; i < l; i++) {

        if(str[i] == c) {
            return 1;
        }
    }
    return 0;
}
