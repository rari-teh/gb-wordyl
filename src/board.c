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

char empty_word_buf[WORD_LENGTH + 1] = "     ";

uint8_t g_board_tile_flip_speed;

/*
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
*/

#define BRD_CRD_Y(y) (BOARD_TILE_Y_START + ((y) * BOARD_TILE_H))
#define BRD_CRD_X(x) (BOARD_TILE_X_START + ((x) * BOARD_TILE_W))
#define BRD_CRD_X_END(x, x_len) (BOARD_TILE_X_START + ((x + x_len) * BOARD_TILE_W))
#define BRD_ROW(y, x, x_len) BRD_CRD_Y(y) , BRD_CRD_X(x) , BRD_CRD_X_END(x, x_len)
#define BRD_CRD_STOP 0xFFu

// Range for consecutive board rows: start offset, length
// This makes a 5 x 6 grid for gameplay
// y, x_start, x_length -> transformed to -> x, x_start, x_end
const uint8_t board_row_ranges_game[] =
  {BRD_ROW(0u, 0u, BOARD_GRID_W),
   BRD_ROW(1u, 0u, BOARD_GRID_W),
   BRD_ROW(2u, 0u, BOARD_GRID_W),
   BRD_ROW(3u, 0u, BOARD_GRID_W),
   BRD_ROW(4u, 0u, BOARD_GRID_W),
   BRD_ROW(5u, 0u, BOARD_GRID_W),
   BRD_CRD_STOP};

// This makes an irregular grid for the intro screen
const uint8_t board_row_ranges_splash[] =
  {BRD_ROW(0u, 1u, 4u), // Game
   BRD_ROW(1u, 3u, 3u), // Boy
   BRD_ROW(2u, 2u, 1u), // Blank
   BRD_ROW(3u, 1u, 4u), // Word..
   BRD_ROW(4u, 0u, 3u), // Blank
   BRD_ROW(4u, 4u, 2u), // ..yl
   BRD_CRD_STOP};

const uint8_t * p_board_layout;

// 2x2 BG metatiles on the board
const uint8_t board_map_letter[]  = {0, 1, 2, 3};

// This is about smaller in code size than the const map version above ^^^
//
// Draw the board tile map
// * Game Board is 5 x 6 array of 2x2 metatiles arranged as first row:0,1 second row: 2,3
// * Also supports aribtrary row starts and lengths
void board_map_fill() {

    const uint8_t * p_range = p_board_layout;
    // const uint8_t * p_range = board_row_ranges_game;
    uint8_t tile_index = BG_TILES_BOARD_LETTERS_START;

    uint8_t * p_board_cgb_addrs = (uint8_t *)board_cgb_addrs;

    while (1) {
        uint8_t y     = *p_range++;
        if (y == BRD_CRD_STOP) break;

        uint8_t start = *p_range++;
        uint8_t end   = *p_range++;

        for (uint8_t x = start; x < end; x += BOARD_TILE_W) {

            // Store x,y coordinates of tile for CGB attribute drawing later
            *p_board_cgb_addrs++ = x;
            *p_board_cgb_addrs++ = y;

            set_bkg_based_tiles(x, y, BOARD_TILE_H, BOARD_TILE_W,
                                board_map_letter, tile_index);
            tile_index += (BOARD_TILE_W * BOARD_TILE_H);
        }
    }

    // board_cgb_addrs_last = board_cgb_addrs_index;
    board_cgb_addrs_last = (tile_index - (BG_TILES_BOARD_LETTERS_START + (BOARD_TILE_W * BOARD_TILE_H))) >> 2;
}




// Tile flip animation frames
const uint8_t board_flip_anim[] = {
    (BOARD_LETTERS_FLIP_1),
    (BOARD_LETTERS_FLIP_2),
    (BOARD_LETTERS_FLIP_3),
    (BOARD_LETTERS_FLIP_4),
    (BOARD_LETTERS_FLIP_3),
    (BOARD_LETTERS_FLIP_2),
    (BOARD_LETTERS_FLIP_1)
};


#define CURSOR_BOARD_X_POS (((BOARD_TILE_X_START * 8)- 16u) + DEVICE_SPRITE_PX_OFFSET_X)

// Move the cursor to highlight the current row
void board_update_cursor(void) {

    // guess_num is desired row
    uint8_t x = CURSOR_BOARD_X_POS;
    uint8_t y = (guess_num * (8u * BOARD_TILE_W)) + (DEVICE_SPRITE_PX_OFFSET_Y + (BOARD_TILE_Y_START * 8u));

    move_sprite(SP_ID_CURSOR_BOARD_START     , x     , y);
    move_sprite(SP_ID_CURSOR_BOARD_START + 1u, x + 8u, y);
    move_sprite(SP_ID_CURSOR_BOARD_START + 2u, x     , y + 8u);
    move_sprite(SP_ID_CURSOR_BOARD_START + 3u, x + 8u, y + 8u);
    // Takes more ROM space:
    /*
    for (uint8_t i = 0; i < SP_ID_CURSOR_BOARD_LEN; i++) {
        move_sprite(SP_ID_CURSOR_BOARD_START + i,
                    CURSOR_BOARD_X_POS + sp_cursor_offset_x[i],
                    y + sp_cursor_offset_y[i]);
    }
    */
}


// Move the cursor to highlight the current row
void board_hide_cursor(void) {

    for (uint8_t i = 0; i < SP_ID_CURSOR_BOARD_LEN; i++) {
        hide_sprite(SP_ID_CURSOR_BOARD_START + i);
    }
}



// Draw the letters for a guess as they are entered
void board_render_guess_letter(uint8_t col) {

    BOARD_SET_FLIP_SPEED(BOARD_TILE_FLIP_FAST);

    // Cheat Mode: Highlight word letters as typed in debug mode
    #ifdef DEBUG_REVEAL_WHILE_TYPE
        // evaluate_letters(guess);
        // board_set_color_for_letter(guess_num, col, BOARD_HIGHLIGHT_YES);
        board_draw_word(guess_num, guess, BOARD_HIGHLIGHT_YES);
    #else
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
//
// Faster flip speeds look ok on all hardware except stock CGB LCD
// which seems to drop some frames?
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
        BOARD_SET_FLIP_SPEED(BOARD_TILE_FLIP_FAST);
    }

    // Flag guess letters as: LETTER_NOT_IN_WORD, LETTER_WRONG_PLACE or LETTER_RIGHT_PLACE
    evaluate_letters(guess);

    // col maps to the individual letters in the word/guess
    for (uint8_t col = 0; col < BOARD_GRID_W; col ++) {

        board_draw_letter(row, col, p_guess[col], do_highlight);
    }
}


// == Lookup tables for colorizing board letters ==

const uint8_t board_cgb_colors[] = {
    SET_BOARD_CGB_PAL_NORMAL,      // LETTER_NOT_SET
    SET_BOARD_CGB_PAL_NOT_IN_WORD, // LETTER_NOT_MATCHED
    SET_BOARD_CGB_PAL_CONTAINS,    // LETTER_WRONG_PLACE
    SET_BOARD_CGB_PAL_MATCHED,     // LETTER_RIGHT_PLACE
};

// DMG color array is 2x colors per entry
const uint8_t board_dmg_colors[] = {
    BOARD_DMG_COLOR_NORMAL,       // LETTER_NOT_SET
    BOARD_DMG_COLOR_NORMAL,       // LETTER_NOT_MATCHED
    BOARD_DMG_COLOR_CONTAINS,     // LETTER_WRONG_PLACE
    BOARD_DMG_COLOR_MATCHED,      // LETTER_RIGHT_PLACE
};


// TODO: optimize this for size/speed
// Set highlight color for a letter on baord based on guess status
void board_set_color_for_letter(uint8_t row, uint8_t col, uint8_t do_highlight) {

    uint8_t match_type = guess_eval[col];

    // CGB: Always used
    // DMG: Used when not highlighting
    SET_PRINT_COLOR_NORMAL;

    if (IS_CGB) {

        // Default to normal style (in case of no highlighting)
        uint8_t color = SET_BOARD_CGB_PAL_NORMAL;


        // If highlighting look up CGB style from LUT
        if (do_highlight) {
            // For CGB, lighten tile is not matched
            if (match_type == LETTER_NOT_MATCHED)
                SET_BOARD_CGB_COLOR_NOT_IN_WORD;

            color = board_cgb_colors[match_type];
        }

        // Apply the CGB coloring
        board_fill_letter_cgb_pal(row, col, color);
    }
    else {
        // DMG mode

        // Override the SET_PRINT_COLOR_NORMAL from above
        if (do_highlight) {
            // DMG color array is 2x colors per entry
            match_type <<= 1;
            set_1bpp_colors(board_dmg_colors[match_type], board_dmg_colors[match_type + 1]);
        }
    }
}


