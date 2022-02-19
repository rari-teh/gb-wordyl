// board.c

#include <gbdk/platform.h>
#include <gbdk/incbin.h>
#include <gb/gbdecompress.h>

#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "common.h"
#include "gfx_common.h"
#include "gameboy_color.h"

#include "keyboard.h"
#include "board.h"


// 5 x 6 array of 2x2 metatiles arranged as first row:0,1 second row: 2,3
const uint8_t board_map[]  = {
 0,1,   4,5,   8, 9, 12,13, 16,17,
 2,3,   6,7,  10,11, 14,15, 18,19,

20,21, 24,25, 28,29, 32,33, 36,37,
22,23, 26,27, 30,31, 34,35, 38,39,

40,41, 44,45, 48,49, 52,53, 56,57,
42,43, 46,47, 50,51, 54,55, 58,59,

60,61, 64,65, 68,69, 72,73, 76,77,
62,63, 66,67, 70,71, 74,75, 78,79,

80,81, 84,85, 88,89, 92,93, 96,97,
82,83, 86,87, 90,91, 94,95, 98,99,

100,101, 104,105, 108,109, 112,113, 116,117,
102,103, 106,107, 110,111, 114,115, 118,119
};

char empty_word_buf[WORD_LENGTH + 1] = "     ";

uint8_t g_board_tile_flip_speed;

// Tile flip animation
const uint8_t board_flip_anim[] = {
    (BOARD_LETTERS_FLIP_1),
    (BOARD_LETTERS_FLIP_2),
    (BOARD_LETTERS_FLIP_3),
    (BOARD_LETTERS_FLIP_2),
    (BOARD_LETTERS_FLIP_1)
};


// Draw the letters for a guess as they are entered
void board_render_guess_letter(uint8_t col) {

    BOARD_SET_FLIP_SPEED(BOARD_TILE_FLIP_FAST);

    // Cheat Mode: Highlight word letters as typed in debug mode
    #ifdef DEBUG_REVEAL_WHILE_TYPE
        // evaluate_letters(guess);
        // board_set_color_for_letter(guess_num, col, BOARD_HIGHLIGHT_YES);
        board_draw_word(guess_num, guess, BOARD_HIGHLIGHT_YES);
    #else
        BOARD_SET_FLIP_SPEED(BOARD_TILE_FLIP_FAST);
        board_draw_letter(guess_num, col, guess[col], BOARD_HIGHLIGHT_NO);
    #endif
}


// Add a guess letter to the board
void board_add_guess_letter(void) {

    uint8_t guess_len = strlen(guess);

    if (guess_len < WORD_LENGTH) {
        guess[guess_len] = keyboard_get_letter();
        board_render_guess_letter(guess_len);
    }
}


// Add a guess letter to the board
void board_remove_guess_letter(void) {

    uint8_t guess_len = strlen(guess);

    if (guess_len > 0) {
        guess[guess_len-1] = GUESS_LETTER_EMPTY;
        board_render_guess_letter(guess_len-1);
    }
}


// Redraw the board for all current guesses
void board_redraw_clean(void) {

    for(int i=0; i < MAX_GUESSES; i++) {
        board_draw_word(i, NULL, BOARD_HIGHLIGHT_NO);
    }
}


// Direct render a tile to VRAM to fill a board letter square
// Previous calls to set_1bpp_colors() will affect colors produced here
void board_draw_letter_bits(uint8_t row, uint8_t col, uint8_t letter) {

    // Draw letter into VRAM address for desired board tile
    // From offset into 1bpp letter source of 4x4 metatiles
    set_bkg_1bpp_data(BG_TILES_BOARD_LETTERS_START
                        + (col * BOARD_GRID_TILES_PER_LETTER)
                        + (row * BOARD_GRID_TILES_PER_ROW),
                      BOARD_GRID_TILES_PER_LETTER,
                      board_letters_decomp_buf + (letter * BOARD_GRID_VRAM_BYTES_PER_LETTER));
}


// Board tile flip animation: Direct render a tile to VRAM to fill a board letter square
// Previous calls to set_1bpp_colors() will affect colors produced here
void board_draw_tile_flip_anim(uint8_t row, uint8_t col) {

    for (uint8_t c = 0; c < ARRAY_LEN(board_flip_anim); c++) {
        board_draw_letter_bits(row, col, board_flip_anim[c]);

        // One frame between animations in all modes
        wait_vbl_done();

        // Another frame of delay in slow mode
        if (g_board_tile_flip_speed == BOARD_TILE_FLIP_SLOW) {
            wait_vbl_done();
        }
    }
}


// Direct render a tile to VRAM to fill a board letter square
// Previous calls to set_1bpp_colors() will affect colors produced here
void board_draw_letter(uint8_t row, uint8_t col, uint8_t letter, bool do_highlight) {

    // Default to normal color scheme
    SET_BOARD_COLOR_NORMAL;

    // If enabled, draw a tile flip animation BEFORE coloring is applied
    #ifndef DEBUG_REVEAL_WHILE_TYPE // Turn off in cheat mode
        if (g_board_tile_flip_speed)
            board_draw_tile_flip_anim(guess_num, col);
    #endif


    if ((letter == ' ') || (letter == 0x00))
        letter = BOARD_LETTERS_SPACE_CHAR;
    else if (letter >= 'a')
        letter -= 'a';
    else if (letter >= 'A')
        letter -= 'A';

    // Only color highlighting when not doing guess entry
    board_set_color_for_letter(row, col, do_highlight);

    // Now draw the letter
    board_draw_letter_bits(row, col, letter);
}



// Render a word at * p_guess onto the board
void board_draw_word(uint8_t row, uint8_t * p_guess, bool do_highlight) {

    // If it's a request to draw an empty word, use a shim empty string
    if (p_guess == NULL) {
        p_guess = empty_word_buf;
        BOARD_SET_FLIP_SPEED(BOARD_TILE_FLIP_NONE);
    } else {
        // Slow tile flip for word reveal
        BOARD_SET_FLIP_SPEED(BOARD_TILE_FLIP_SLOW);
    }

    // Flag guess letters as: LETTER_NOT_IN_WORD, LETTER_WRONG_PLACE or LETTER_RIGHT_PLACE
    evaluate_letters(guess);

    // col maps to the individual letters in the word/guess
    for (uint8_t col = 0; col < BOARD_GRID_W; col ++) {

        board_draw_letter(row, col, p_guess[col], do_highlight);
    }
}


// Set highlight color for a letter on baord based on guess status
void board_set_color_for_letter(uint8_t row, uint8_t col, uint8_t do_highlight) {

    if (IS_CGB)
        SET_PRINT_COLOR_NORMAL;

    // If highlighting is turned off just use normal style
    if (!do_highlight) {

        if (IS_CGB)
            board_fill_letter_cgb_pal(row, col, SET_BOARD_CGB_PAL_NORMAL);
        else
            SET_BOARD_COLOR_NORMAL;
    } else {

        if (guess_eval[col] == LETTER_RIGHT_PLACE) {

            if (IS_CGB) {
                board_fill_letter_cgb_pal(row, col, SET_BOARD_CGB_PAL_MATCHED);
            }
            else
                SET_BOARD_COLOR_MATCHED;

        } else if (guess_eval[col] == LETTER_WRONG_PLACE) {

            if (IS_CGB)
                board_fill_letter_cgb_pal(row, col, SET_BOARD_CGB_PAL_CONTAINS);
            else
                SET_BOARD_COLOR_CONTAINS;

        } else { // implied: if (guess_eval[col] == LETTER_NOT_IN_WORD) {

            if (IS_CGB)
                board_fill_letter_cgb_pal(row, col, SET_BOARD_CGB_PAL_NORMAL);
            else
                SET_BOARD_COLOR_NORMAL;
        }
    }
}


// Set up graphics for gameplay
void board_initgfx(void) {

    // Map Data
    SET_PRINT_COLOR_NORMAL;

    // Clear screen
    fill_bkg_rect(0, 0, DEVICE_SCREEN_WIDTH, DEVICE_SCREEN_HEIGHT, BG_TILES_BLANK_START);

    // Load 2bpp blank tile
    gb_decompress_bkg_data((BG_TILES_BLANK_START), tile_blank);

    // == Font Numbers ==
    // Load 1bpp font num tiles - borrow font_letters_decomp_buf for a moment
    gb_decompress(font_num_tiles, font_letters_decomp_buf);
    // Load tiles into vram for font printing
    set_bkg_1bpp_data(BG_TILES_FONT_NUM_START, BG_TILES_FONT_NUM_LEN, font_letters_decomp_buf);

    // == Font Letters ==
    // Load 1bpp font tiles (used by both keyboard for VRAM drawing and print as a VRAM tileset)
    gb_decompress(font_tiles, font_letters_decomp_buf);
    // Load tiles into vram for font printing
    set_bkg_1bpp_data(BG_TILES_FONT_START, BG_TILES_FONT_LEN, font_letters_decomp_buf);

    // == Game Board Letter Tiles / Squares ==
    // Decompress board letter tiles into a buffer, they get written to VRAM later as needed
    gb_decompress(board_letter_tiles, board_letters_decomp_buf);

    // == Dialog Window Tiles / Squares ==
    // Load 2bpp window dialog tiles
    gb_decompress_bkg_data((BG_TILES_DIALOG_START), dialog_tiles);

    // == Draw the game board map ==
    // TODO: this + map takes up about 300 bytes, optimize further? seems like setup call for this is what costs the most
    // Set up Board Letter map in VRAM
    // (direct addressable for rewriting letters via changing tile contents)
    set_bkg_based_tiles(BOARD_TILE_X_START, BOARD_TILE_Y_START,
                        BOARD_GRID_TILE_W, BOARD_GRID_TILE_H,
                        board_map,
                        BG_TILES_BOARD_LETTERS_START);

    // Set up colors for board area
    if (IS_CGB)
        board_initgfx_cgb();

    // TODO: replace me
    print_gotoxy(2,0, PRINT_BKG);
    print_str("GAME BOY WORDYL");

    // Sprite Data
    // Load 2bpp blank tile
    set_sprite_data((SP_TILES_CURSOR_START), SP_TILES_CURSOR_LEN, letter_cursor_tiles);

    SPRITES_8x8;

    for (uint8_t i = 0; i < SP_ID_CURSOR_LEN; i++) {
        set_sprite_tile(SP_ID_CURSOR_START + i, SP_TILES_CURSOR_START);
        set_sprite_prop(SP_ID_CURSOR_START + i, sp_cursor_props[i]);
    }

    // Clear window and move it offscreen at the bottom
    move_win(0 + WIN_X_OFFSET, DEVICE_SCREEN_PX_HEIGHT); // Window is offscreen by default
    fill_win_rect(0, 0, DEVICE_SCREEN_WIDTH, DEVICE_SCREEN_HEIGHT, BG_TILES_BLANK_START );
}
