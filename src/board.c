// board.c

#include <gbdk/platform.h>
#include <string.h>
#include <stdint.h>
#include <gbdk/incbin.h>
#include <gb/gbdecompress.h>

#include "word-db.h"

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

const uint8_t empty_word_buf[] = {' ', ' ', ' ', ' ', ' '};


// Draw the current guessed word to the board
void board_render_guess(void) {

    SET_BOARD_COLOR_NORMAL; // TODO: remove?
    board_draw_word(guess_num, guess, BOARD_HIGHLIGHT_NO); // guess_num is row
}


// Redraw the board for all current guesses
void board_redraw(void) {

    char * p_guess;

    for(uint8_t i = 0; i < MAX_GUESSES; i++) {
        // Draw words up to end of guesses, then blank words after
        if (i < guess_num)
            p_guess = guesses[i];
        else
            p_guess = NULL;
        board_draw_word(i, p_guess, BOARD_HIGHLIGHT_YES);
    }
}


// Direct render a tile to VRAM to fill a board letter square
// Previous calls to set_1bpp_colors() will affect colors produced here
void board_draw_letter(uint8_t row, uint8_t col, uint8_t letter) {

    if ((letter == ' ') || (letter == 0x00))
        letter = BOARD_LETTERS_SPACE_CHAR;
    else if (letter >= 'a')
        letter -= 'a';
    else if (letter >= 'A')
        letter -= 'A';

    // Draw letter into VRAM address for desired board tile
    // From offset into 1bpp letter source of 4x4 metatiles
    set_bkg_1bpp_data(BG_TILES_BOARD_LETTERS_START
                        + (col * BOARD_GRID_TILES_PER_LETTER)
                        + (row * BOARD_GRID_TILES_PER_ROW),
                      BOARD_GRID_TILES_PER_LETTER,
                      board_letters_decomp_buf + (letter * BOARD_GRID_VRAM_BYTES_PER_LETTER));
}


// Render a word at * p_guess onto the board
void board_draw_word(uint8_t row, uint8_t * p_guess, bool do_highlight) {

    if (p_guess == NULL) {
        p_guess = empty_word_buf;
        SET_BOARD_COLOR_NORMAL;
    }

    // Cheat Mode: Highlight word letters as typed in debug mode
    #ifdef DEBUG_ON
        do_highlight = true;
    #endif

    // col maps to the individual letters in the word/guess
    for (uint8_t col = 0; col < BOARD_GRID_W; col ++) {

        board_set_color_for_letter(row, col, word, p_guess[col], do_highlight);

        board_draw_letter(row, col, p_guess[col]);
    }
}


// Set highlight color for a letter on baord based on guess status
void board_set_color_for_letter(uint8_t row, uint8_t col, char *word, char letter, uint8_t do_highlight) {

    if (IS_CGB)
        SET_PRINT_COLOR_NORMAL;

    if (!do_highlight) {
        if (IS_CGB)
            board_fill_letter_cgb_pal(row, col, SET_BOARD_CGB_PAL_NORMAL);
        else
            SET_BOARD_COLOR_NORMAL;
    }
    else if(word[col] == letter) {
        if (IS_CGB) {
            board_fill_letter_cgb_pal(row, col, SET_BOARD_CGB_PAL_MATCHED);
        }
        else
            SET_BOARD_COLOR_MATCHED;

    // } else if (contains(guessed_wrong, letter)) {
    //     SET_BOARD_COLOR_NOT_IN_WORD;

    } else if (contains(word, letter)) {
        if (IS_CGB)
            board_fill_letter_cgb_pal(row, col, SET_BOARD_CGB_PAL_CONTAINS);
        else
            SET_BOARD_COLOR_CONTAINS;

    } else {
        if (IS_CGB)
            board_fill_letter_cgb_pal(row, col, SET_BOARD_CGB_PAL_NORMAL);
        else
            SET_BOARD_COLOR_NORMAL;
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

    // TODO: fixme, broken
    print_gotoxy(2,0, PRINT_BKG);
    print_str("GAME BOY WORDLE");

    // Sprite Data
    // Load 2bpp blank tile
    set_sprite_data((SP_TILES_CURSOR_START), SP_TILES_CURSOR_LEN, letter_cursor_tiles);

    SPRITES_8x8;

    for (uint8_t i = 0; i < SP_ID_CURSOR_LEN; i++) {
        set_sprite_tile(SP_ID_CURSOR_START + i, SP_TILES_CURSOR_START);
        set_sprite_prop(SP_ID_CURSOR_START + i, sp_cursor_props[i]);
    }

    // Center screen
    move_bkg(252, 252); // TODO: handle offsets for sprites and raw bkg display

    // Clear window and move it offscreen at the bottom
    move_win(0 + WIN_X_OFFSET, DEVICE_SCREEN_PX_HEIGHT); // Window is offscreen by default
    fill_win_rect(0, 0, DEVICE_SCREEN_WIDTH, DEVICE_SCREEN_HEIGHT, BG_TILES_BLANK_START );
    SHOW_WIN;

    SHOW_BKG;
    SHOW_SPRITES;
    DISPLAY_ON;
}