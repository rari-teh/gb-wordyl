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
const char *kb[KEYBD_ROWS] = {
"QWERTYUIOP",
 "ASDFGHJKL",
  "ZXCVBNM"};

// Stores color coding for keyboard basde on guess data
uint8_t kb_status[KEYBD_ROWS][10];


// Length of each keyboard row in characters
int8_t kb_coords[KEYBD_ROWS] = {
    10,
    9,
    7
};

// Which tile column each keyboard row starts on
int8_t kb_offsets[KEYBD_ROWS] = {
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
void keyboard_set_color_for_letter(uint8_t row, uint8_t col, uint8_t match_type, uint8_t tile_id) {

    // Reject color letter downgrades (only allowed after a keyboard status reset)
    // Otherwise update status
    if (match_type < kb_status[row][col])
        return;
    else
        (kb_status[row][col] = match_type);

    // CGB doesn't use DMG style VRAM tile redrawing except on
    // clean redraw where it uses normal print style
    if (IS_CGB)
        SET_PRINT_COLOR_NORMAL;

    if (match_type == LETTER_RIGHT_PLACE) {

        if (IS_CGB)
            keyboard_fill_letter_cgb_pal(row, col, SET_KEYBD_CGB_PAL_MATCHED);
        else
            SET_KEYBD_COLOR_MATCHED;
    }
    else if (match_type == LETTER_WRONG_PLACE) {

        if (IS_CGB)
            keyboard_fill_letter_cgb_pal(row, col, SET_KEYBD_CGB_PAL_CONTAINS);
        else
            SET_KEYBD_COLOR_CONTAINS;
    }
    else if (match_type == LETTER_NOT_MATCHED) {
        if (IS_CGB)
            keyboard_fill_letter_cgb_pal(row, col, SET_KEYBD_CGB_PAL_NOT_IN_WORD);
        else
            SET_KEYBD_COLOR_NOT_IN_WORD;
    }
    else { // implied: if (match_type == LETTER_NOT_SET) {

        // Keyboard default: no highlight
        if (IS_CGB)
            keyboard_fill_letter_cgb_pal(row, col, SET_KEYBD_CGB_PAL_NORMAL);
        else
            SET_KEYBD_COLOR_NORMAL;
    }

    // Only needed for DMG unless it's a clean redraw since CGB just updates the attribute map
    if ((!IS_CGB) || (match_type == LETTER_NOT_SET))
        draw_letter_to_tileid(kb[row][col], tile_id);
}


// Update keyboard highlighting based on a guessed word evaluation
// TODO: could be optimized
void keyboard_update_from_guess(void) {

    uint8_t highest_match_type;
    uint8_t tile_id = BG_TILES_KEYBD_START;

    // Loop through keyboard, check for letters to highlight
    for(uint8_t row = 0; row < 3; row++) {

        uint8_t kbl = strlen(kb[row]);
        for(uint8_t col=0; col < kbl; col++) {

            char letter = kb[row][col];

            // See if current keyboard letter is in guess word
            highest_match_type = LETTER_NOT_MATCHED;

            for (int c = 0; c < WORD_LENGTH; c++) {

                if (letter == guess[c]) {

                    // find highest match type in the guess word
                    if (guess_eval[c] > highest_match_type)
                        highest_match_type = guess_eval[c];

                    keyboard_set_color_for_letter(row, col, highest_match_type, tile_id);
                }
            }
            tile_id++;
        }
    }
}


// Reset / Redraw the entire keyboard with no letters highlighted
void keyboard_redraw_clean(void) {

    uint8_t tile_id = BG_TILES_KEYBD_START;
    for(uint8_t row = 0; row < 3; row++) {

        uint8_t kbl = strlen(kb[row]);
        for(uint8_t col=0; col < kbl; col++) {

            // Reset keyboard status
            kb_status[row][col] = LETTER_NOT_SET;

            keyboard_set_color_for_letter(row, col, LETTER_NOT_SET, tile_id);
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

    // Bar across top of keyboard
    fill_bkg_rect(KEYBD_START_X, KEYBD_START_Y-1, 33, 1, BG_TILES_DIALOG_START + DIALOG_TILE_TOP);
}


// Move the cursor to highlight a key
void keyboard_update_cursor(void) {

    uint8_t x = (kb_x * 16) + (kb_offsets[kb_y] * 8) + DEVICE_SPRITE_PX_OFFSET_X;
    uint8_t y = ((KEYBD_START_Y + kb_y) * 8) + DEVICE_SPRITE_PX_OFFSET_Y - KEYBD_CURSOR_OFFSET_Y;

    for (uint8_t i = 0; i < SP_ID_CURSOR_LEN; i++) {
        move_sprite(SP_ID_CURSOR_START + i, x + sp_cursor_offset_x[i], y + sp_cursor_offset_y[i] );
    }
}


void keyboard_move_cursor(int8_t move_x, int8_t move_y) {

    // Update Y first (may change X) and handle wraparound
    if (move_y != 0) {
        kb_y += move_y;

        if (kb_y < 0) {
            kb_y = ARRAY_LEN(kb) - 1;
        }
        else if (kb_y >= ARRAY_LEN(kb)) {
            kb_y = 0;
        }

        // Bump X to end or row if it's shorter X if needed on row change
        if(kb_x >= kb_coords[kb_y]) {
            kb_x = kb_coords[kb_y] - 1;
        }
    }


    // Update X and handle wraparound
    kb_x += move_x;

    if (kb_x >= kb_coords[kb_y])
        kb_x = 0;
    else if (kb_x < 0)
        kb_x = kb_coords[kb_y] - 1;


    keyboard_update_cursor();
}