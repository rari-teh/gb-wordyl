// common.c

#include <gbdk/platform.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "common.h"

uint8_t guess_eval[WORD_LENGTH];
bool answer_letter_used[WORD_LENGTH];


uint8_t guess_num;

char guess[WORD_LENGTH+1];
char guesses[WORD_LENGTH+1][MAX_GUESSES];

// TODO: implement?
// char guessed_wrong[30];
// char guessed_position[30];
// char guessed_correct[30];
char word[WORD_LENGTH+1];


/*
// TODO: DEAD CODE
// Check if a letter is in a string
uint8_t contains(char *str, char c) {

    uint8_t len = strlen(str);
    uint8_t count = 0;

    for(uint8_t i=0; i < len; i++) {
        if(str[i] == c) {
            count++;
        }
    }
    return count;
}
*/


// Check a guess word for letter matches and marks
// them as LETTER_NOT_IN_WORD, LETTER_WRONG_PLACE or LETTER_RIGHT_PLACE
// Original: arpruss. Optimized some
void evaluate_letters(char* guess) {

    // First scan word for exact letter matches
    for (uint8_t i=0; i < WORD_LENGTH; i++) {

        if (guess[i] == word[i]) {
            guess_eval[i] = LETTER_RIGHT_PLACE;
            answer_letter_used[i] = true;
        }
        else {
            guess_eval[i] = LETTER_NOT_MATCHED;
            answer_letter_used[i] = false;
        }
    }

    // Next for each guess char check RIGHT Letter at WRONG location
    for (uint8_t i=0; i < WORD_LENGTH; i++) {

        if (guess_eval[i] == LETTER_NOT_MATCHED) {

            // Scan through unused answer word letters
            for (uint8_t j=0; j < WORD_LENGTH;j++) {

                if ((answer_letter_used[j] == false) && (guess[i] == word[j])) {

                    // Found a match at WRONG location, mark both and
                    // break out for next char in guess
                    answer_letter_used[j] = true;
                    guess_eval[i] = LETTER_WRONG_PLACE;
                    break;
                }
            }
        }
    }
}
