// gameplay.c

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
#include "stats.h"


uint8_t game_won_str[] = "You won!\n\nOn Guess X of 6";


// Show a popup message: You Won
void show_win_message(uint8_t guess_count) {

    // Patch the number of guesses into the string at char 'X'
    uint8_t * p_str = game_won_str;
    while (*p_str != 'X') p_str++;
    *p_str = guess_count + '0';

    win_dialog_show_message(DIALOG_WON_MESSAGE_WIN_Y, game_won_str, NULL);

    // Restore X marker char
    *p_str = 'X';
}


// Show a popup message: You Lost
void show_lose_message(char *correct_word) {

    win_dialog_show_message(DIALOG_LOSE_MESSAGE_WIN_Y, "You lost. Sorry!\n\nAnswer is: ", correct_word);
}


// Process a player guess for a word
uint8_t gameplay_handle_guess(void) {

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

    board_draw_word(guess_num, guess, BOARD_HIGHLIGHT_YES);
    guess_num += 1;
    keyboard_update_from_guess();
    keyboard_update_cursor();

    // == Handle Game Over scenarios ==

    // Check for correct match
    bool game_won = (strcmp(word, guess) == 0);
    if (game_won) {
        show_win_message(guess_num);
        stats_update(game_won, guess_num); // TODO: consolidate handling
        return GAME_STATE_RESTART;
    }
    else if (guess_num == MAX_GUESSES) {
        show_lose_message(word);
        stats_update(game_won, guess_num); // TODO: consolidate handling
        return GAME_STATE_RESTART;
    }

    // Reset guess to empty and prepare for next one
    memset(guess, 0, WORD_LENGTH);

    return STATUS_GAME_CONTINUE;
}


// Choose an answer word for a new gameplay round
// Modifies global: word
void gameplay_init_answer_word(void) {

    uint16_t seed = LY_REG;
    seed |= (uint16_t)DIV_REG << 8;
    initrand(seed);
    int r = rand();

    // TODO: FIXME? Is this limiting to first 211 answer words?
    while(r > 211) {
        r = rand();
    }
    #ifdef DEBUG_FORCE_WORD_BY_NUM
        r = DEBUG_FORCE_WORD_BY_NUM;
    #endif
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


// Run once on startup to prepare gameplay board graphics
void gameplay_init(void) {

    board_initgfx();
    keyboard_draw_map();

    // Initialize Window dialog box
    win_dialog_draw();

    // Draws initial empty board and keyboard
    board_redraw_clean();
    keyboard_reset();

    // Center screen by scrolling slightly to the left
    // move_bkg(252, 252);
    move_bkg(252, 0);

    SHOW_WIN;
    SHOW_BKG;
    SHOW_SPRITES;
    DISPLAY_ON;

    // Reset guess to empty and prepare for next one
    strcpy(word, "EMPTY");
}


// Runs on startup and before start of a new gameplay round
void gameplay_restart(void) {

    guess_num = 0;
    memset(guess, 0, sizeof(guess));

    // Draws initial empty board and keyboard
    board_redraw_clean();
    keyboard_reset();

    // Wait for first button press then Initialize answer word ~randomly
    while (!(joypad() & J_ANY_KEY)) {
        wait_vbl_done();
    }
    gameplay_init_answer_word();
}


// Runs the main gameplay: word entry, answer checking, messaging
void gameplay_run(void)
{

    while(1) {
        wait_vbl_done();

        UPDATE_KEYS();
        UPDATE_KEY_REPEAT(J_DPAD);

        // Handle D-Pad movement separate from other buttons
        // so that buttons kept held down don't lock out D-pad
        //  movement during keyboard entry
        if (KEY_PRESSED(J_DPAD)) {

            // Check for initial button press or repeat
            if ((key_repeat_count == KEY_REPEAT_START) ||
                (key_repeat_count >= KEY_REPEAT_DPAD_THRESHOLD)) {

                if (key_repeat_count >= KEY_REPEAT_DPAD_THRESHOLD)
                    RESET_KEY_REPEAT(KEY_REPEAT_DPAD_RELOAD_VAL);

                // Filter with D-Pad to allow movement while still pressing A / B
                switch(keys & J_DPAD) {
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
        }

        if (KEY_TICKED(J_A | J_B | J_SELECT | J_START)) {
            switch(keys) {

                case J_SELECT:
                    break;

                // Check a guess
                case J_START:
                    // TODO: better handling of game over/won
                    if (gameplay_handle_guess() != STATUS_GAME_CONTINUE) {
                        game_state = GAME_STATE_OVER; // TODO: better location for this?
                        return; // Game was Won or Lost, exit
                    }
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
    }
}
