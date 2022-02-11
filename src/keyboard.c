// keyboard.c

#include <gbdk/platform.h>
#include <string.h>
#include <stdint.h>

#include "word-db.h"

#include "common.h"
#include "gfx_common.h"
#include "gameboy_color.h"

#include "keyboard.h"

// Keyboard map
const char *kb[3] = {
"QWERTYUIOP",
 "ASDFGHJKL",
  "ZXCVBNM"};

// Length of each keyboard row in characters
int8_t kb_coords[3] = {
    10,
    9,
    7
};

// Which tile column each keyboard row starts on
int8_t kb_offsets[3] = {
    0,
    1,
    2
};

int8_t kb_x = 0;
int8_t kb_y = 0;


// Cursor sprite flipping flags to allow use of same tile for all 4 corners
const uint8_t sp_cursor_props[] = { 0x00, S_FLIPX, S_FLIPY, S_FLIPX | S_FLIPY };
const uint8_t sp_cursor_offset_x[] = { 0, 8, 0, 8 };
const uint8_t sp_cursor_offset_y[] = { 0, 0, 8, 8 };


// return currently selected letter in keyboard
inline char keyboard_get_letter(void) {

    return kb[kb_y][kb_x];
}


// Set the CGB color for a keyboard letter tile
void keyboard_fill_letter_cgb_pal(uint8_t row, uint8_t col, uint8_t palnum) {

    // Select CGB attribute tile mode for bkg map access
    VBK_REG = VBKF_BKG_ATTRIB;

    // Fill region with requested CGB palette attribute
    set_bkg_tile_xy(KEYBD_START_X + (col * 2) + kb_offsets[row], KEYBD_START_Y + row, palnum & CGB_PAL_MASK);

    // Return to normal tile BKG mode
    VBK_REG = VBKF_BKG_TILES;
}


// Set highlight color for a letter on the keyboard based on guess status
void keyboard_set_color_for_letter(uint8_t row, uint8_t col, char letter) {

    if (IS_CGB)
        SET_PRINT_COLOR_NORMAL;

    // if(letter == ' ')
    // {
    //     set_1bpp_colors(DMG_BLACK, DMG_WHITE);
    // }
    if (contains(guessed_wrong, letter)) {
        if (IS_CGB)
            keyboard_fill_letter_cgb_pal(row, col, SET_KEYBD_CGB_PAL_NOT_IN_WORD);
        else
            SET_KEYBD_COLOR_NOT_IN_WORD;
    }
    else if(contains(guessed_position, letter)) {
        if (IS_CGB)
            keyboard_fill_letter_cgb_pal(row, col, SET_KEYBD_CGB_PAL_CONTAINS);
        else
            SET_KEYBD_COLOR_CONTAINS;
    }
    else if(contains(guessed_correct, letter)) {
        if (IS_CGB)
            keyboard_fill_letter_cgb_pal(row, col, SET_KEYBD_CGB_PAL_MATCHED);
        else
            SET_KEYBD_COLOR_MATCHED;
    }
    else {
        if (IS_CGB)
            keyboard_fill_letter_cgb_pal(row, col, SET_KEYBD_CGB_PAL_NORMAL);
        else
            SET_KEYBD_COLOR_NORMAL;
    }
}


// TODO: OPTIMIZE: add function to just redraw a single keyboard letter on demand instead of entire board

// Redraw they keyboard with letters highlighted based on guess data
void keyboard_redraw() {

    uint8_t tile_id = BG_TILES_KEYBD_START;
    for(uint8_t row = 0; row < 3; row++) {

        uint8_t kbl = strlen(kb[row]);
        for(uint8_t col=0; col < kbl; col++) {

            char letter = kb[row][col];
            keyboard_set_color_for_letter(row, col, letter);
            draw_letter_to_tileid(letter, tile_id);
            tile_id++;
        }
    }
}


// Draw a map of incrementing VRAM tiles for the keyboard
// These get drawn into later
void keyboard_draw_map(void) {

    uint8_t tile_id = BG_TILES_KEYBD_START;

    for(uint8_t row = 0; row < 3; row++) {
        uint8_t kbl = strlen(kb[row]);

        // Add a blank space between every horizontal letter
        for(uint8_t col=0; col < kbl * 2; col += 2) {
            set_bkg_tile_xy(KEYBD_START_X + col + kb_offsets[row], KEYBD_START_Y + row, tile_id);
            tile_id++;
        }
    }
}


// Move the cursor to highlight a key
void keyboard_highlight_letter(void) {

    uint8_t x = (kb_x * 16) + (kb_offsets[kb_y] * 8) + DEVICE_SPRITE_PX_OFFSET_X;
    uint8_t y = ((KEYBD_START_Y + kb_y) * 8) + DEVICE_SPRITE_PX_OFFSET_Y;

    for (uint8_t i = 0; i < SP_ID_CURSOR_LEN; i++) {
        move_sprite(SP_ID_CURSOR_START + i, x + sp_cursor_offset_x[i], y + sp_cursor_offset_y[i] );
    }
}
