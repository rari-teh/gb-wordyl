/* Wordle clone for Game Boy.
 * A weekend project by stacksmashing.
 * Contact: twitter.com/ghidraninja
 *          code@stacksmashing.net
 *
 * Dictionary improvements by zeta_two
 *
 * CGB + GFX refactor improvement by bbbbbr
 */
#include <gbdk/platform.h>
#include <gbdk/incbin.h>
#include <gb/gbdecompress.h>

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <rand.h>

// #include "word-db.h"
#include "decode.h"

#include "common.h"
#include "input.h"
#include "gfx_common.h"
#include "gameboy_color.h"

#include "keyboard.h"
#include "board.h"
#include "window.h"


uint8_t game_handle_guess(void);
void game_init_answer_word(void);


uint8_t game_won_str[] = "You won!\n\nOn Guess X of 6";

// Show a popup message: You Won
void show_win_message(uint8_t guess_count) {

    // Patch the number of guesses into the string at char 'X'
    uint8_t * p_str = game_won_str;
    while (*p_str != 'X') p_str++;
    *p_str = guess_count + '0';

    win_dialog_show_message(DIALOG_WON_MESSAGE_WIN_Y, game_won_str, NULL);
    reset(); // TODO: replace and improve, move to function calling this
}

// Show a popup message: You Lost
void show_lose_message(char *correct_word) {

    win_dialog_show_message(DIALOG_LOSE_MESSAGE_WIN_Y, "You lost. Sorry!\n\nAnswer is: ", correct_word);
    reset(); // TODO: replace and improve, move to function calling this
}

/*
// TODO: DEAD CODE?
void analyze_guess(char *guess) {
    for(int i = 0; i < WORD_LENGTH; i++) {
        if(guess[i] == word[i]) {
            guessed_correct[strlen(guessed_correct)] = guess[i];
        } else if(contains(word, guess[i])) {
            guessed_position[strlen(guessed_position)] = guess[i];
        } else {
            guessed_wrong[strlen(guessed_wrong)] = guess[i];
        }
    }
}
*/

uint8_t game_handle_guess(void) {

    if (strlen(guess) != WORD_LENGTH) {
        // Insufficient length
        win_dialog_show_message(DIALOG_GAME_INFO_Y, "Word is too short!\n\nNeeds 5 Letters", NULL);
        return STATUS_GAME_CONTINUE;
    }
    else if (!filterWord(guess)) {
        // Word not in dictionary
        win_dialog_show_message(DIALOG_GAME_INFO_Y, "Word is not in\n\ndictionary!", NULL);
        return STATUS_GAME_CONTINUE;
    }

    // analyze_guess(guess); // TODO: DEAD CODE?
    // strcpy(guesses[guess_num], guess);

    board_draw_word(guess_num, guess, BOARD_HIGHLIGHT_YES);
    guess_num += 1;
    keyboard_update_from_guess();
    keyboard_update_cursor();

    // Check for correct match
    if(strcmp(word, guess) == 0) {
        show_win_message(guess_num);
        return STATUS_GAME_WON;
    }
    if(guess_num == MAX_GUESSES) {
        // show_loose();
        show_lose_message(word);
        return STATUS_GAME_LOST;
    }

    // Reset guess to empty and prepare for next one
    memset(guess, 0, WORD_LENGTH);

    return STATUS_GAME_CONTINUE;
}


void game_init_answer_word(void) {

    uint16_t seed = LY_REG;
    seed |= (uint16_t)DIV_REG << 8;
    initrand(seed);
    int r = rand();

    // TODO: FIXME? Is this limiting to first 211 answer words?
    while(r > 211) {
        r = rand();
    }
    // get_word(r, word);
    getSpecialWord(r, word);

    #ifdef DEBUG_FORCE_WORD
        strcpy(word, DEBUG_FORCE_WORD);
    #endif

    #ifdef DEBUG_REVEAL_WORD
        // Cheat Mode: Display answer word in debug mode
        print_gotoxy(0,0, PRINT_BKG);
        print_str(word);
    #endif
}


void run_wordyl(void)
{
    board_initgfx();

    // Initialize Window dialog box
    win_dialog_draw();

    strcpy(word, "EMPTY");

    guess_num = 0;
    memset(guess, 0, sizeof(guess));
    // TODO: DEAD CODE?
    // memset(guessed_wrong, 0, sizeof(guessed_wrong));
    // memset(guessed_position, 0, sizeof(guessed_position));
    // memset(guessed_correct, 0, sizeof(guessed_correct));

    // TODO: DEAD CODE?
    // for(uint8_t i=0; i < MAX_GUESSES; i++) {
    //     strcpy(guesses[i], "");
    // }

    // Draws initial empty board
    board_redraw_clean();

    keyboard_draw_map();
    keyboard_redraw_clean();

    // Show cursor on default keyboard key
    keyboard_update_cursor();

    // Wait for first button press then Initialize answer word
    while (!(joypad() & J_ANY_KEY)) {
        wait_vbl_done();
    }
    game_init_answer_word();

    while(1) {
        UPDATE_KEYS();


        // Handle D-Pad movement separate from other buttons
        // so that buttons kept held down don't lock out D-pad
        //  movement during keyboard entry
        if (KEY_TICKED(J_LEFT | J_RIGHT | J_UP | J_DOWN)) {
            // Filter D-pad buttons pressed to only test changed ones
            switch(keys & keys_changed) {
                    // Keyboard Movement
                    case J_RIGHT:
                        keyboard_move_cursor(1, 0);
                        break;

                    case J_LEFT:
                        keyboard_move_cursor(-1, 0);
                        break;

                    case J_UP:
                        keyboard_move_cursor(0, -1);
                        break;

                    case J_DOWN:
                        keyboard_move_cursor(0, 1);
                        break;

            }
        }

        if (KEY_TICKED(J_A | J_B | J_SELECT | J_START)) {
            switch(keys) {

                case J_SELECT:
                    break;

                // Check a guess
                case J_START:
                    // TODO: better handling of game over/won
                    if (game_handle_guess() != STATUS_GAME_CONTINUE)
                        return; // Game was Won or Lost, exit
                    break;

                // Add/Remove letters from a guess
                case J_A:
                    board_add_guess_letter();
                    break;

                case J_B:
                    board_remove_guess_letter();
                    break;
            }
        }

        wait_vbl_done();
    }
}


void main() {

    cgb_check_and_init();

    while(1) {
        run_wordyl();
    }
}
