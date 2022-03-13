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

#include "gameplay.h"
#include <lang_text.h>

#define GAMEPLAY_SET_GAMEOVER  game_state = GAME_STATE_OVER


// TODO: dialogs.c

void show_intro_message(void) {

    win_dialog_show_message(INTRO_DIALOG_WIN_Y, __INTRO_MESSAGE_STR, NULL);
}

// Should not be const, gets modified
uint8_t game_won_str[] = __MESSAGE_GAME_WON_STR;

// Show a popup message: You Won
void show_win_message(uint8_t guess_count) {

    // Patch the number of guesses into the string at char '^'
    uint8_t * p_str = game_won_str;
    while (*p_str != '^') p_str++;
    *p_str = guess_count + '0';

    win_dialog_show_message(WON_MESSAGE_DIALOG_WIN_Y, game_won_str, NULL);

    // Restore X marker char
    *p_str = '^';
}


// Show a popup message: You Lost
void show_lose_message(char *correct_word) {

    win_dialog_show_message(LOSE_MESSAGE_DIALOG_WIN_Y, __MESSAGE_LOSE_STR, correct_word);
}


void show_options_message(void) {

    uint8_t  ret_keys_ticked;

    ret_keys_ticked = win_dialog_show_message(OPTIONS_MENU_DIALOG_WIN_Y, __OPTIONS_MENU_STR, NULL);
    switch (ret_keys_ticked) {
        case J_RIGHT:
            stats_reset();
            win_dialog_show_message(STATS_RESET_DIALOG_WIN_Y, __MESSAGE_STATS_RESET_STR ,NULL);
            // Fall through to show stats
        case J_B:
            stats_show();
            break;

        case J_UP:
            // sets: GAMEPLAY_SET_GAMEOVER
            gameplay_handle_lose();
            break;
    }
}


void gameplay_handle_lose(void) {
    // Hide cursor so it doesn't flash between popups
    board_hide_cursor();
    show_lose_message(word);
    stats_update(GAME_NOT_WON, guess_num);
    GAMEPLAY_SET_GAMEOVER;
}


// Process a player guess for a word (including winning and losing)
//
// * If the player won or lost, then game state
//   will be changed to game over, causing an exit
//   from the main gameplay loop
void gameplay_handle_guess(void) {

    // TODO: special handling for word_len = 0 -> don't send popup?
    if (strlen(guess) != WORD_LENGTH) {

        // Insufficient length
        win_dialog_show_message(WORD_TOO_SHORT_DIALOG_WIN_Y, __MESSAGE_WORD_TOO_SHORT_STR, NULL);
    }
    else if (!filterWord(guess)) {

        // Word not in dictionary
        win_dialog_show_message(WORD_NOT_IN_DICT_DIALOG_WIN_Y, __MESSAGE_WORD_NOT_IN_DICT_STR, NULL);
    } else {

        // Otherwise process the guess word

        board_draw_word(guess_num, guess, BOARD_HIGHLIGHT_YES);
        guess_num += 1;
        keyboard_update_from_guess();
        // keyboard_update_cursor();

        // == Handle Game Over scenarios ==

        // Check for correct match
        bool game_was_won = (strcmp(word, guess) == 0);

        if (game_was_won) {
            // Hide cursor so it doesn't flash between popups
            board_hide_cursor();
            show_win_message(guess_num);
            stats_update(GAME_WAS_WON, guess_num);
            GAMEPLAY_SET_GAMEOVER;
        }
        else if (guess_num == MAX_GUESSES) {
            // sets: GAMEPLAY_SET_GAMEOVER;
            gameplay_handle_lose();
        } else {
            board_update_cursor();
        }

        // Reset guess to empty and prepare for next one
        memset(guess, 0, WORD_LENGTH);
    }
}


// Choose an answer word for a new gameplay round
// Modifies global: word
void gameplay_init_answer_word(void) {

    uint16_t r = randw() % NUM_ANSWERS;

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


// Run once on startup to prepare gameplay board graphics maps
//
// This DOES NOT draw the actual graphics, just lays out maps
// so they can be drawn using draw-via-tiles instead of draw-via-tile-ids
void gameplay_init_maps(void) {

    // == Setup the game board & keyboard ==

    // Clear screen
    fill_bkg_rect(0, 0, DEVICE_SCREEN_WIDTH, DEVICE_SCREEN_HEIGHT, BG_TILES_BLANK_START);
    keyboard_draw_map();

    // Draw stars
    // IMPORTANT: relies on stars already being in decompressed map
    //     gb_decompress(intro_stars_map, map_decomp_buf);
    set_bkg_based_tiles(0,0,  DEVICE_SCREEN_WIDTH - 2, DEVICE_SCREEN_HEIGHT - KEYBD_ROWS -2 , map_decomp_buf, BG_TILES_INTRO_DIALOG_START);

    // Set up Board Letter map in VRAM
    // (direct addressable for rewriting letters via changing tile contents)
    board_map_fill();
    // set_bkg_based_tiles(BOARD_TILE_X_START, BOARD_TILE_Y_START,
    //                     BOARD_GRID_TILE_W, BOARD_GRID_TILE_H,
    //                     board_map,
    //                     BG_TILES_BOARD_LETTERS_START);

    // Set up colors for board area
    if (IS_CGB)
        board_initgfx_cgb();
}


// Runs on startup and before start of a new gameplay round
void gameplay_restart(void) {

    guess_num = 0;
    memset(guess, 0, sizeof(guess));

    // Draws initial empty board and keyboard
    board_redraw_clean();
    board_update_cursor();

    keyboard_reset();

    // print_gotoxy(4u,0, PRINT_BKG);
    // print_str("gb  wordyl\n");
    // print_gotoxy(19u - 6u, 0u, PRINT_BKG);
    // print_str("bbbbbr\n");

}


// Runs the main gameplay: word entry, answer checking, messaging
void gameplay_run(void)
{

    while(game_state == GAME_STATE_RUNNING) {
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

                // Show Options menu
                case J_SELECT:
                    // Can set game_state to lost (exiting loop)
                    show_options_message();
                    break;

                // Check a guess
                case J_START:
                    // Can set game_state to won / lost (exiting loop)
                    gameplay_handle_guess();
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
