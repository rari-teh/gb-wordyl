// common.c

#include <gbdk/platform.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "common.h"

uint8_t game_state;

uint8_t guess_eval[WORD_LENGTH];
bool answer_letter_used[WORD_LENGTH];

uint8_t guess_num;

char guess[WORD_LENGTH+1];
char word[WORD_LENGTH+1];  // TODO: rename this to "answer_word"


#define PRINT_MAX_DIGITS  5

// Patch a number into the string at char 'X'
// * uitoa() puts in leading zeros and a tailing str terminator, so it's not suitable
// * Warning: Safetys are turned off, lots can go wrong here
uint8_t * str_u16_left_at_X(uint8_t * p_str, uint16_t num) {

    uint8_t index = PRINT_MAX_DIGITS;

    // if (p_str == NULL) return NULL;

    // Search for start position
    while ((*p_str != 'X') && (*p_str)) p_str++;

    // Store individual digits of n in reverse order
    // Starting at the END of the array and working forward
    // (using do-while to handle when initial value == 0)
    do {
        // decrement the counter first, so it finishes as pointing to the current digit in the array
        index--;
        *p_str-- = (num % 10) + '0';
        num = num / 10;
    } while (num != 0);

    // Fill remaining characters with space chars
    while (index > 0) {
        index--;
        *p_str-- = ' ';
    }

    // Move pointer past end of print area
    return (p_str + PRINT_MAX_DIGITS + 1);
}



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
