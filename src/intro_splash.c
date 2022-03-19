// gameplay.c

#include <gbdk/platform.h>
#include <gb/gbdecompress.h>

#include <stdint.h>
#include <stdbool.h>
#include <rand.h>

#include "common.h"
#include "input.h"
#include "gfx_common.h"
#include "gameboy_color.h"
#include "fade.h"

#include "board.h"
#include "intro_splash.h"

#include <lang_text.h>


void splash_run(void) {

    DMG_PAL_INVERT_ON;

    // == INITIAL CREDITS SCREEN ==
    fade_in();

    print_gotoxy(__INTRO_CREDITS_X, __INTRO_CREDITS_Y, PRINT_BKG);
    print_str(__INTRO_CREDITS_STR);
    if (IS_CGB) delay(4000);
    else        delay(2000);
    fade_out();


    // == MAIN SPLASH SCREEN ==

    // Scroll up one tile row to move the board down a little
    move_bkg(0, (uint8_t)-8); // TODO: macro?
    BOARD_SET_LAYOUT_SPLASH;
    splash_init_maps();

    // Use an inverted pallette for splash screen
    fade_in();

    splash_animate_title();
    waitpadticked_lowcpu(J_START);

    fade_out();
    // Revert to normal palette
    DMG_PAL_INVERT_OFF;

    move_bkg(252, 0); // TODO: macro
}


// Run once on startup to prepare gameplay board graphics
void splash_init_maps(void) {

    // IMPORTANT: Expects dialog border map to already be
    // decompressed and ready
    //     gb_decompress(intro_dialog_map, map_decomp_buf);
    set_bkg_based_tiles(0,31, DEVICE_SCREEN_WIDTH, DEVICE_SCREEN_HEIGHT, map_decomp_buf, BG_TILES_INTRO_DIALOG_START);

    // Decompress stars map
    gb_decompress(intro_stars_map, map_decomp_buf);
    set_bkg_based_tiles(1,0,  DEVICE_SCREEN_WIDTH - 2, DEVICE_SCREEN_HEIGHT - 1, map_decomp_buf, BG_TILES_INTRO_DIALOG_START);

    // Set up Board Letter map in VRAM
    // (direct addressable for rewriting letters via changing tile contents)
    board_map_fill();
    // set_bkg_based_tiles(BOARD_TILE_X_START, BOARD_TILE_Y_START,
    //                     BOARD_GRID_TILE_W, BOARD_GRID_TILE_H,
    //                     board_map,
    //                     BG_TILES_BOARD_LETTERS_START);

    board_redraw_clean();

    if (IS_CGB)
        splash_initgfx_cgb();

    print_gotoxy((DEVICE_SCREEN_WIDTH - (sizeof(__INTRO_PRESS_START_STR) - 1)) / 2u,DEVICE_SCREEN_HEIGHT - 4u, PRINT_BKG);
    print_str(__INTRO_PRESS_START_STR);
}

#define CHR_NUM(c) (c - 'A')

// "GAMEBOY WORDYL";
const uint8_t splash_text[] = {
        CHR_NUM('G'),
        CHR_NUM('A'),
        CHR_NUM('M'),
        CHR_NUM('E'),
        CHR_NUM('B'),
        CHR_NUM('O'),
        CHR_NUM('Y'),
        BOARD_LETTERS_SPACE_CHAR,
        CHR_NUM('W'),
        CHR_NUM('O'),
        CHR_NUM('R'),
        CHR_NUM('D'),
        BOARD_LETTERS_SPACE_CHAR,
        BOARD_LETTERS_SPACE_CHAR,
        BOARD_LETTERS_SPACE_CHAR,
        CHR_NUM('Y'),
        CHR_NUM('L'),};

const uint8_t splash_text_color[] = {
    CGB_PAL_GREEN, // GAME
    CGB_PAL_GREEN,
    CGB_PAL_GREEN,
    CGB_PAL_GREEN,
    CGB_PAL_GREEN, // BOY
    CGB_PAL_GREEN,
    CGB_PAL_GREEN,

    CGB_PAL_WHITE, // SPACE

    CGB_PAL_BLUE, // WORD
    CGB_PAL_BLUE,
    CGB_PAL_BLUE,
    CGB_PAL_BLUE,

    CGB_PAL_WHITE, // SPACE
    CGB_PAL_WHITE, // SPACE
    CGB_PAL_WHITE, // SPACE

    CGB_PAL_BLUE,  // YL
    CGB_PAL_BLUE};


void splash_animate_title(void) {

    uint8_t row = 0;
    uint8_t col = 0;
    bool skip_anim = false;

    // BOARD_SET_FLIP_SPEED(BOARD_TILE_FLIP_FAST);
    BOARD_SET_FLIP_SPEED(BOARD_TILE_FLIP_SLOW);

    for (uint8_t c = 0; c < ARRAY_LEN(splash_text); c++) {

        // Allow user to slip through animation
        UPDATE_KEYS();
        if (KEY_PRESSED(J_ANY_KEY))
            skip_anim = true;

        // Don't show letter flip animation for space chars
        // and don't change their board tile color
        if (splash_text[c] != BOARD_LETTERS_SPACE_CHAR) {
            if (!skip_anim) board_draw_tile_flip_anim(row, col);
            SET_BOARD_COLOR_MATCHED;
        }
        else
            SET_BOARD_COLOR_NORMAL;

        if (IS_CGB)
            board_fill_letter_cgb_pal(row, col, splash_text_color[c]);

        board_draw_letter_bits(row, col, splash_text[c]);

        // Simulate transition between board rows as
        // letters increment, to re-use board drawing logic
        row++;
        if (row = BOARD_GRID_W) {
            row = 0;
            col++;
        }
    }

}
