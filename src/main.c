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
// #include <gbdk/font.h>
#include <string.h>
#include <stdint.h>
#include <rand.h>
#include <gbdk/incbin.h>
#include <gb/gbdecompress.h>

#include "word-db.h"

#include "common.h"
#include "gfx_common.h"
#include "gameboy_color.h"

#include "keyboard.h"
#include "board.h"
#include "window.h"


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

void run_wordyl(void)
{
    board_initgfx();

    // Initialize Window dialog box
    win_dialog_draw();

    strcpy(word, "EMPTY");
    int has_random = 0;

    guess_num = 0;
    memset(guess, 0, sizeof(guess));
    // TODO: DEAD CODE?
    // memset(guessed_wrong, 0, sizeof(guessed_wrong));
    // memset(guessed_position, 0, sizeof(guessed_position));
    // memset(guessed_correct, 0, sizeof(guessed_correct));

    for(int i=0; i < MAX_GUESSES; i++) {
        strcpy(guesses[i], "");
    }

    // Draws initial empty board
    board_redraw_clean();

    keyboard_draw_map();
    keyboard_redraw_clean();

    // Show cursor on default keyboard key
    keyboard_update_cursor();
    while(1) {
        int j = joypad();
        if((has_random == 0) && (j != 0)) {
            uint16_t seed = LY_REG;
            seed |= (uint16_t)DIV_REG << 8;
            initrand(seed);
            int r = rand();
            while(r > 211) {
                r = rand();
            }
            get_word(r, word);

            #ifdef DEBUG_FORCE_WORD
                strcpy(word, DEBUG_FORCE_WORD);
            #endif

            has_random = 1;

            #ifdef DEBUG_REVEAL_WORD
                // Cheat Mode: Display answer word in debug mode
                print_gotoxy(0,0, PRINT_BKG);
                print_str(word);
            #endif
        }

        switch(j) {
            case J_RIGHT:
                kb_x += 1;
                if(kb_x >= kb_coords[kb_y]) {
                    kb_x = 0;
                }
                keyboard_update_cursor();
                waitpadup();
                break;
            case J_LEFT:
                kb_x -= 1;
                if(kb_x < 0) {
                    kb_x = kb_coords[kb_y] - 1;
                }
                keyboard_update_cursor();
                waitpadup();
                break;
            case J_UP:
                kb_y -= 1;
                if(kb_y < 0) {
                    kb_y = 2;
                }
                if(kb_x >= kb_coords[kb_y]) {
                    kb_x = kb_coords[kb_y] - 1;
                }
                keyboard_update_cursor();
                waitpadup();
                break;
            case J_DOWN:
                kb_y += 1;
                if(kb_y > 2) {
                    kb_y = 0;
                }
                if(kb_x >= kb_coords[kb_y]) {
                    kb_x = kb_coords[kb_y] - 1;
                }
                keyboard_update_cursor();
                waitpadup();
                break;
            case J_SELECT:
            case J_START:
                if(strlen(guess) != WORD_LENGTH) {
                    // TODO: indicate insufficient length
                    break;
                }
                else if(!query_word(guess)) {
                    // TODO: indicate not a matched word
                    break;
                }
                // analyze_guess(guess); // TODO: DEAD CODE?
                strcpy(guesses[guess_num], guess);
                board_draw_word(guess_num, guess, BOARD_HIGHLIGHT_YES);
                guess_num += 1;
                keyboard_update_from_guess();
                keyboard_update_cursor();
                if(strcmp(word, guess) == 0) {
                    show_win_message(guess_num);
                    return;
                    break;
                }
                if(guess_num == MAX_GUESSES) {
                    // show_loose();
                    show_lose_message(word);
                    return;
                    break;
                }
                // empty guess
                memset(guess, 0, WORD_LENGTH);
                // TODO
                break;
            case J_A:
                if (strlen(guess) == WORD_LENGTH) break; // TODO: replace with counter
                guess[strlen(guess)] = keyboard_get_letter();
                board_render_guess_entry();
                waitpadup();
                break;
            case J_B:
                if (strlen(guess) == 0) break;
                guess[strlen(guess)-1] = 0;
                board_render_guess_entry();
                waitpadup();
                break;
            default:
            break;
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
