/* Wordle for Game Boy.
 * A weekend project by stacksmashing.
 * Contact: twitter.com/ghidraninja
 *          code@stacksmashing.net
 */
#include <gb/gb.h>
#include <gb/drawing.h>

#include <gbdk/console.h>
#include <gbdk/font.h>
#include <string.h>
#include <stdint.h>
#include <rand.h>

#include <gbdk/incbin.h>
#include <gb/gbdecompress.h>

#include "word-db.h"

// Sprite Data
#include "../res/letter_cursor_tiles.h"

// Map Data
//#include "../res/board_grid_tiles_gbcompress.h"
#include "../res/font_tiles_1bpp_gbcompress.h"
#include "../res/font_num_tiles_1bpp_gbcompressed.h"
// INCBIN(board_grid_map, "res/board_grid_map_gbcompress.bin")
// INCBIN_EXTERN(board_grid_map)
#include "../res/board_letter_tiles_8x8_1bpp_gbcomp.h"

// Window / Dialog data
#include "../res/dialog_tiles_2bpp_gbcompressed.h"

// #define DEBUG_ON

// Hardware correction factor for X position of Window when displayed on screen
#define WIN_X_OFFSET 7

// Cursor sprite flipping flags to allow use of same tile for all 4 corners
const uint8_t sp_cursor_props[] = { 0x00, S_FLIPX, S_FLIPY, S_FLIPX | S_FLIPY };
const uint8_t sp_cursor_offset_x[] = { 0, 8, 0, 8 };
const uint8_t sp_cursor_offset_y[] = { 0, 0, 8, 8 };

#define SP_TILES_CURSOR_START 0u
#define SP_TILES_CURSOR_LEN   1u

#define SP_ID_CURSOR_START 0u
#define SP_ID_CURSOR_LEN   4u





// Blank tile, could be optimized
const uint8_t tile_blank[] = {0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00};



// TODO: COUNT needs to be BOARD SIZE IN TILES (5 * 6 * 4)
#define BOARD_LETTERS_BYTES_PER_TILE  8u // 1bpp tiles = 8 bytes per 8x8 tile
#define BOARD_LETTERS_COUNT          27u // Last entry is blank space
#define BOARD_LETTERS_TILES_PER       4u // Last entry is blank space
// Empty space is last char in board font tile set
#define BOARD_LETTERS_SPACE_CHAR (BOARD_LETTERS_COUNT - 1)

uint8_t board_letters_decomp_buf[BOARD_LETTERS_COUNT * BOARD_LETTERS_TILES_PER * BOARD_LETTERS_BYTES_PER_TILE];

#define FONT_LETTERS_COUNT           29u // 26 letters + 3 special chars: ".", "!", ":"
#define FONT_LETTERS_BYTES_PER_TILE   8u // 1bpp tiles = 8 bytes per 8x8 tile

uint8_t font_letters_decomp_buf[BOARD_LETTERS_COUNT * FONT_LETTERS_BYTES_PER_TILE];


#define BOARD_GRID_X  2u // Start x,y in Tiles
#define BOARD_GRID_Y  1u
#define BOARD_GRID_W  5u // Size in Tiles
#define BOARD_GRID_H  6u
#define BOARD_TILE_W  2u
#define BOARD_TILE_H  2u
#define BOARD_GRID_TILES_PER_LETTER 4u
#define BOARD_GRID_VRAM_BYTES_PER_LETTER (BOARD_LETTERS_BYTES_PER_TILE * BOARD_GRID_TILES_PER_LETTER)
#define BOARD_GRID_TILES_PER_ROW    (BOARD_GRID_W * BOARD_GRID_TILES_PER_LETTER)
#define BOARD_GRID_TILE_W (BOARD_GRID_W * BOARD_TILE_W)
#define BOARD_GRID_TILE_H (BOARD_GRID_H * BOARD_TILE_H)

#define BOARD_TILE_X_START (BOARD_GRID_X)
#define BOARD_TILE_Y_START (BOARD_GRID_Y)


// ==== VRAM tile assignment for Gameplay Screen ====
#define BG_TILES_BLANK_START 0u
#define BG_TILES_BLANK_LEN   1u

#define BG_TILES_KEYBD_START  (BG_TILES_BLANK_START + BG_TILES_BLANK_LEN)
#define BG_TILES_KEYBD_LEN    26u // 26 letters

#define BG_TILES_BOARD_LETTERS_START (BG_TILES_KEYBD_START + BG_TILES_KEYBD_LEN)
#define BG_TILES_BOARD_LETTERS_LEN   (BOARD_GRID_W * BOARD_GRID_H * BOARD_GRID_TILES_PER_LETTER)

#define BG_TILES_DIALOG_START (BG_TILES_BOARD_LETTERS_START + BG_TILES_BOARD_LETTERS_LEN)
#define BG_TILES_DIALOG_LEN   5u

#define BG_TILES_FONT_START   (BG_TILES_DIALOG_START + BG_TILES_DIALOG_LEN)
#define BG_TILES_FONT_LEN     29u // 26 letters + 3 special chars: ".", "!", ":"
#define BG_TILES_FONT_PERIOD  (BG_TILES_FONT_START + 26u)
#define BG_TILES_FONT_EXCLAIM (BG_TILES_FONT_START + 27u)
#define BG_TILES_FONT_COLON   (BG_TILES_FONT_START + 28u)

#define BG_TILES_FONT_NUM_START   (BG_TILES_FONT_START + BG_TILES_FONT_LEN)
#define BG_TILES_FONT_NUM_LEN     10u // 10 digits


#define DIALOG_TILE_L    4u
#define DIALOG_TILE_UL   0u
#define DIALOG_TILE_R    3u
#define DIALOG_TILE_UR   2u
#define DIALOG_TILE_TOP  1u



// 5 x 6 array of metatiles arranged as first row:0,1 second row: 2,3
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



/*
// Board Letter box colors: Foreground, Background
// Standard color scheme
// Printing
#define SET_PRINT_COLOR_NORMAL       set_1bpp_colors(DMG_BLACK, DMG_WHITE)     // Full contrast text
// Board
#define SET_BOARD_COLOR_NORMAL       set_1bpp_colors(DMG_BLACK, DMG_WHITE)     // Full contrast text
#define SET_BOARD_COLOR_CONTAINS     set_1bpp_colors(DMG_BLACK, DMG_LITE_GRAY) // Filled in box text
#define SET_BOARD_COLOR_MATCHED      set_1bpp_colors(DMG_WHITE, DMG_BLACK)     // Inverted text
// #define SET_BOARD_COLOR_NOT_IN_WORD  set_1bpp_colors(DMG_LITE_GRAY, DMG_WHITE)  // Inverted text
// Keyboard
#define SET_KEYBD_COLOR_NORMAL       set_1bpp_colors(DMG_BLACK, DMG_WHITE)  // Faded text
#define SET_KEYBD_COLOR_CONTAINS     set_1bpp_colors(DMG_BLACK, DMG_LITE_GRAY)      // Full contrast text
#define SET_KEYBD_COLOR_MATCHED      set_1bpp_colors(DMG_WHITE, DMG_BLACK)  // Inverted text
#define SET_KEYBD_COLOR_NOT_IN_WORD  set_1bpp_colors(DMG_LITE_GRAY, DMG_WHITE)  // Inverted text
*/

// Board Letter box colors: Foreground, Background
// Alt color scheme (lighter board, less inversion)
// Printing
#define SET_PRINT_COLOR_NORMAL       set_1bpp_colors(DMG_BLACK, DMG_WHITE)     // Full contrast text
// Board
#define SET_BOARD_COLOR_NORMAL       set_1bpp_colors(DMG_LITE_GRAY, DMG_WHITE)  // Faded text
#define SET_BOARD_COLOR_CONTAINS     set_1bpp_colors(DMG_BLACK, DMG_WHITE)      // Full contrast text
#define SET_BOARD_COLOR_MATCHED      set_1bpp_colors(DMG_BLACK, DMG_LITE_GRAY)  // Inverted text
// #define SET_BOARD_COLOR_NOT_IN_WORD  set_1bpp_colors(DMG_WHITE, DMG_LITE_GRAY)  // Inverted text
// Keyboard
#define SET_KEYBD_COLOR_NORMAL       set_1bpp_colors(DMG_LITE_GRAY, DMG_WHITE)  // Faded text
#define SET_KEYBD_COLOR_CONTAINS     set_1bpp_colors(DMG_BLACK, DMG_WHITE)      // Full contrast text
#define SET_KEYBD_COLOR_MATCHED      set_1bpp_colors(DMG_BLACK, DMG_LITE_GRAY)  // Inverted text
#define SET_KEYBD_COLOR_NOT_IN_WORD  set_1bpp_colors(DMG_WHITE, DMG_LITE_GRAY)  // Inverted text





#define PRINT_BKG 0
#define PRINT_WIN 1
uint8_t * print_vram_addr = NULL;



void print_gotoxy(uint8_t x, uint8_t y, uint8_t target) {
    if (target == PRINT_BKG)
        print_vram_addr = get_bkg_xy_addr(x,y);
    else
        print_vram_addr = get_win_xy_addr(x,y);
}

#define BOARD_HIGHLIGHT_NO  false
#define BOARD_HIGHLIGHT_YES true

// gfx_print.c

/*
// TODO: DELETE IF NOT IN USE ANYMORE - switch to on-demand printinf to vram or, re-add the font tiles if there is space
// TODO: OPTIMIZE?
// Draws a character (only handles A - Z)
// Advances to next VRAM location
void print_char(char letter) {

    if (letter == ' ') {
        set_vram_byte( print_vram_addr, BG_TILES_BLANK_START );
    }
    else if ((letter >= 'A') && (letter <= 'Z')) {
        set_vram_byte( print_vram_addr, (letter - 'A') + BG_TILES_FONT_START);
    }
    print_vram_addr++;
}*/

// gfx_print.c

// TODO: DELETE IF NOT IN USE ANYMORE - switch to on-demand printinf to vram or, re-add the font tiles if there is space
void print_str(char * txt) {

    uint8_t letter;
    uint8_t line_wrap_x_addr = (((uint8_t)print_vram_addr) & 0x1Fu);// (uint8_t)((uint16_t)print_vram_addr & 0x001Fu);

    while(*txt) {

        if (*txt == '\n') {
            // Mask out X location, reset it to line wrap location, move to next line
            print_vram_addr = (uint16_t *)(((uint16_t)print_vram_addr & 0xFFE0u) + (line_wrap_x_addr + 0x20u));
            txt++;
            continue;
        }
        else if(*txt >= 'A' && *txt <= 'Z') {
            letter = BG_TILES_FONT_START + (unsigned char)(*txt - 'A');
        } else if(*txt >= 'a' && *txt <= 'z') {
            letter = BG_TILES_FONT_START + (unsigned char)(*txt - 'a');
        } else if(*txt >= '0' && *txt <= '9') {
            letter = BG_TILES_FONT_NUM_START + (unsigned char)(*txt - '0');
        } else if(*txt == '.')
            letter = BG_TILES_FONT_PERIOD;
        else if(*txt == '!')
            letter = BG_TILES_FONT_EXCLAIM;
        else if(*txt == ':')
            letter = BG_TILES_FONT_COLON;
        else {
            // Default is blank tile for Space or any other unknown chars
            letter = BG_TILES_BLANK_START;
        }

        set_vram_byte(print_vram_addr++, letter);
        txt++; // Next character
    }
}


// Draw dialog box outline on the window
void win_dialog_draw(void) {

    // Top corners
    set_win_tile_xy(0,0, BG_TILES_DIALOG_START + DIALOG_TILE_UL);
    set_win_tile_xy(DEVICE_SCREEN_WIDTH - 1,0, BG_TILES_DIALOG_START + DIALOG_TILE_UR);
    // Top bar
    fill_win_rect(1, 0, DEVICE_SCREEN_WIDTH - 2, 1, BG_TILES_DIALOG_START + DIALOG_TILE_TOP);
    // Left and right edges
    fill_win_rect(0, 1, 1, DEVICE_SCREEN_HEIGHT - 1, BG_TILES_DIALOG_START + DIALOG_TILE_L);
    fill_win_rect(DEVICE_SCREEN_WIDTH - 1, 1, 1, DEVICE_SCREEN_HEIGHT - 1, BG_TILES_DIALOG_START + DIALOG_TILE_R);
}


// Draw dialog box outline on the window
void win_dialog_show_message(uint8_t win_y_moveto, uint8_t * str_1, uint8_t * str_2) {
    uint8_t win_y_save = WY_REG;
    uint8_t scroll_amt;

    // Clear dialog content area
    fill_win_rect(1, 1, DEVICE_SCREEN_WIDTH-2, DEVICE_SCREEN_HEIGHT-1, BG_TILES_BLANK_START );

    HIDE_SPRITES;

    // Show message
    print_gotoxy(1,1, PRINT_WIN);
    print_str(str_1);

    // Optional second message
    if (str_2)
        print_str(str_2);

    // Scroll window into view (with a small ease-in)
    while (WY_REG > win_y_moveto) {
        if ((WY_REG - win_y_moveto) > 20u)
            scroll_amt = 6u;
        else if ((WY_REG - win_y_moveto) > 10u)
            scroll_amt = 2u;
        else
            scroll_amt = 1u;

        WY_REG -= scroll_amt;
        wait_vbl_done();
    }

    // Wait for button press and then release
    while (!(joypad() & (J_A | J_B | J_START))) {
        wait_vbl_done();
    }
    while ((joypad() & (J_A | J_B | J_START))) {
        wait_vbl_done();
    }

    // Scroll window out of view (with a small ease-out)
    while (WY_REG < win_y_save) {
        if ((WY_REG - win_y_moveto) <= 8u)
            scroll_amt = 1u;
        else if ((WY_REG - win_y_moveto) <= 18u)
            scroll_amt = 2u;
        else
            scroll_amt = 6u;

        WY_REG += scroll_amt;
        wait_vbl_done();
    }
    SHOW_SPRITES;
}



void init_gfx_gameplay(void) {


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

    // TODO: or, move this into a function draw_board_map()
    // TODO: this + map takes up about 300 bytes, optimize further? seems like setup call for this is what costs the most
    // Set up Board Letter map in VRAM
    // (direct addressable for rewriting letters via changing tile contents)
    set_bkg_based_tiles(BOARD_TILE_X_START, BOARD_TILE_Y_START,
                        BOARD_GRID_TILE_W, BOARD_GRID_TILE_H,
                        board_map,
                        BG_TILES_BOARD_LETTERS_START);

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

    move_win(0 + WIN_X_OFFSET, DEVICE_SCREEN_PX_HEIGHT); // Window is offscreen by default
    fill_win_rect(0, 0, DEVICE_SCREEN_WIDTH, DEVICE_SCREEN_HEIGHT, BG_TILES_BLANK_START );
    SHOW_WIN;

    SHOW_BKG;
    SHOW_SPRITES;
    DISPLAY_ON;
}




#define MAX_GUESSES 6

void set_box_color_for_letter(char *word, int position, char letter);
void set_board_color_for_letter(char *word, int position, char letter);

// Keyboard map
const char *kb[3] = {
"QWERTYUIOP",
 "ASDFGHJKL",
  "ZXCVBNM"};
// Length of each keyboard row in characters
int kb_coords[3] = {
    10,
    9,
    7
};
// Which tile column each keyboard row starts on
int kb_offsets[3] = {
    0,
    1,
    2
};

int8_t kb_x = 0;
int8_t kb_y = 0;
uint8_t guess_nr;
char guess[WORD_LENGTH+1];
char guesses[WORD_LENGTH+1][MAX_GUESSES];
char guessed_wrong[30];
char guessed_position[30];
char guessed_correct[30];
char word[WORD_LENGTH+1];


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


const uint8_t empty_word_buf[] = {' ', ' ', ' ', ' ', ' '};

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
        if (do_highlight)
            set_board_color_for_letter(word, col, p_guess[col]);
        board_draw_letter(row, col, p_guess[col]);
    }
}


int contains(char *str, char c) {
    int l = strlen(str);
    for(int i=0; i < l; i++) {
        if(str[i] == c) {
            return 1;
        }
    }
    return 0;
}



// Deprecated
void set_box_color_for_letter(char *word, int position, char letter) {

    // if(word[position] == letter) {
    //     set_1bpp_colors(DMG_DARK_BLACK, DMG_WHITE);

    // } else if(contains(word, letter)) {
    //     set_1bpp_colors(DMG_LITE_GRAY, DMG_WHITE);

    // } else {
    //     set_1bpp_colors(DMG_BLACK, DMG_WHITE);
    // }

/*
    if(word[position] == letter) {
        color(BLACK, BLACK, M_FILL);
    } else if(contains(word, letter)) {
        color(BLACK, DKGREY, M_FILL);
    } else {
        color(BLACK, WHITE, M_NOFILL);
    }
*/
}


void set_board_color_for_letter(char *word, int position, char letter) {

    if(word[position] == letter) {
        SET_BOARD_COLOR_MATCHED;

    // } else if (contains(guessed_wrong, letter)) {
    //     SET_BOARD_COLOR_NOT_IN_WORD;

    } else if (contains(word, letter)) {
        SET_BOARD_COLOR_CONTAINS;

    } else {
        SET_BOARD_COLOR_NORMAL;
    }

/*    if(word[position] == letter) {
        color(WHITE, BLACK, M_NOFILL);
    } else if(contains(word, letter)) {
        color(WHITE, DKGREY, M_NOFILL);
    } else {
        color(BLACK, WHITE, M_NOFILL);
    }
*/
}

// Unused?
void set_color_for_gletter(char letter) {

/*    if(letter == ' ') {
        color(BLACK, WHITE, M_NOFILL);
    } else if (contains(guessed_wrong, letter)) {
        color(BLACK, WHITE, M_NOFILL);
    } else if(contains(guessed_position, letter)) {
        color(WHITE, WHITE, M_NOFILL);
    } else if(contains(guessed_correct, letter)) {
        color(WHITE, WHITE, M_NOFILL);
    } else {
        color(BLACK, WHITE, M_NOFILL);
    }
*/
}


// TODO: keyboard.c

void set_color_for_letter(char letter) {
    // if(letter == ' ')
    // {
    //     set_1bpp_colors(DMG_BLACK, DMG_WHITE);
    // }
    if (contains(guessed_wrong, letter))
    {
        SET_KEYBD_COLOR_NOT_IN_WORD;
        //set_1bpp_colors(DMG_LITE_GRAY, DMG_WHITE);
    }
    else if(contains(guessed_position, letter))
    {
        SET_KEYBD_COLOR_CONTAINS;
        // set_1bpp_colors(DMG_DARK_GRAY, DMG_WHITE);
    }
    else if(contains(guessed_correct, letter))
    {
        SET_KEYBD_COLOR_MATCHED;
        // set_1bpp_colors(DMG_DARK_GRAY, DMG_WHITE);
    }
    else {
        SET_KEYBD_COLOR_NORMAL;
        // set_1bpp_colors(DMG_BLACK, DMG_WHITE);
    }

/*    if(letter == ' ') {
        color(BLACK, WHITE, M_NOFILL);
    } else if (contains(guessed_wrong, letter)) {
        color(LTGREY, WHITE, M_NOFILL);
    } else if(contains(guessed_position, letter)) {
        color(DKGREY, WHITE, M_NOFILL);
    } else if(contains(guessed_correct, letter)) {
        color(DKGREY, WHITE, M_NOFILL);
    } else {
        color(BLACK, WHITE, M_NOFILL);
    }
*/
}

#define KEYBD_START_X     0u // Starting Tile Col of Keyboard
#define KEYBD_START_Y    14u // Starting Tile Row of Keyboard

// keyboard.c
//
// Draw a map of incrementing VRAM tiles for the keyboard
// These get drawn into later
void draw_keyboard_map(void) {

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


// gfx.c

// Direct render a keyboard letter to VRAM to fill a keyboard entry
// Previous calls to set_1bpp_colors() will affect colors produced here
// Keyboard entries are 1 byte per entry on the map
void draw_letter_to_tileid(uint8_t letter, uint8_t index) {

    // if ((letter == ' ') || (letter == 0x00)) letter = BOARD_LETTERS_SPACE_CHAR;
    if (letter >= 'a') letter -= 'a';
    else if (letter >= 'A') letter -= 'A';

    // Draw letter into VRAM address for desired keyboard tile
    set_bkg_1bpp_data(index, 1, font_letters_decomp_buf + (letter * FONT_LETTERS_BYTES_PER_TILE));
}

// keyboard.c

// TODO: remove x,y from keyboard parameters
// Redraw they keyboard with letters highlighted based on guess data
void draw_keyboard() {

    uint8_t tile_id = BG_TILES_KEYBD_START;
    for(uint8_t row = 0; row < 3; row++) {

        uint8_t kbl = strlen(kb[row]);
        for(uint8_t col=0; col < kbl; col++) {

            char letter = kb[row][col];
            set_color_for_letter(letter);
            draw_letter_to_tileid(letter, tile_id);
            tile_id++;
        }
    }
}



// Move the cursor to highlight a key
void highlight_key() {

    uint8_t x = (kb_x * 16) + (kb_offsets[kb_y] * 8) + DEVICE_SPRITE_PX_OFFSET_X;
    uint8_t y = ((KEYBD_START_Y + kb_y) * 8) + DEVICE_SPRITE_PX_OFFSET_Y;

    for (uint8_t i = 0; i < SP_ID_CURSOR_LEN; i++) {
        move_sprite(SP_ID_CURSOR_START + i, x + sp_cursor_offset_x[i], y + sp_cursor_offset_y[i] );
    }
}

char getletter() {
    return kb[kb_y][kb_x];
    //return kb[kb_y][kb_offsets[kb_y] + (kb_x*2)];
}


void render_guess() {

    SET_BOARD_COLOR_NORMAL;
    board_draw_word(guess_nr, guess, BOARD_HIGHLIGHT_NO); // guess_nr is row
}


// ---------------------------------------------- FIX ME

void draw_board() {

    char * p_guess;

    for(uint8_t i = 0; i < MAX_GUESSES; i++) {
        // Draw words up to end of guesses, then blank words after
        if (i < guess_nr)
            p_guess = guesses[i];
        else
            p_guess = NULL;
        board_draw_word(i, p_guess, BOARD_HIGHLIGHT_YES);
    }
}



#define DIALOG_WON_MESSAGE_WIN_Y  (DEVICE_SCREEN_PX_HEIGHT - (8 * 4.5)) // 4.5 rows tall
#define DIALOG_LOSE_MESSAGE_WIN_Y  (DIALOG_WON_MESSAGE_WIN_Y)

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


void run_wordle(void)
{
    init_gfx_gameplay();

    strcpy(word, "EMPTY");
    int has_random = 0;

    guess_nr = 0;
    memset(guess, 0, sizeof(guess));
    memset(guessed_wrong, 0, sizeof(guessed_wrong));
    memset(guessed_position, 0, sizeof(guessed_position));
    memset(guessed_correct, 0, sizeof(guessed_correct));

    for(int i=0; i < MAX_GUESSES; i++) {
        strcpy(guesses[i], "");
    }

    // Initialize Window dialog box
    win_dialog_draw();

    // Draws initial empty board
    for(int i=0; i < MAX_GUESSES; i++) {
        board_draw_word(i, NULL, BOARD_HIGHLIGHT_NO);
    }

/*
    gotogxy(2, 0);
    gprint("GameBoy  WORDLE");
*/
    draw_keyboard_map();
    draw_keyboard();

    // Show cursor on default keyboard key
    highlight_key();
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
            has_random = 1;
        }

        switch(j) {
            case J_RIGHT:
                kb_x += 1;
                if(kb_x >= kb_coords[kb_y]) {
                    kb_x = 0;
                }
                highlight_key();
                waitpadup();
                break;
            case J_LEFT:
                kb_x -= 1;
                if(kb_x < 0) {
                    kb_x = kb_coords[kb_y] - 1;
                }
                highlight_key();
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
                highlight_key();
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
                highlight_key();
                waitpadup();
                break;
            case J_SELECT:
            case J_START:
                if(strlen(guess) != WORD_LENGTH) break;
                if(!query_word(guess)) break;
                analyze_guess(guess);
                strcpy(guesses[guess_nr], guess);
                guess_nr += 1;
                draw_board();
                draw_keyboard();
                highlight_key();
                if(strcmp(word, guess) == 0) {
                    show_win_message(guess_nr);
                    return;
                    break;
                }
                if(guess_nr == MAX_GUESSES) {
                    // show_loose();
                    show_lose_message(word);
                    return;
                    break;
                }
                // empty guess
                memset(guess, 0, 5);
                // TODO
                break;
            case J_A:
                if(strlen(guess) == WORD_LENGTH) break;
                guess[strlen(guess)] = getletter();
                render_guess();
                waitpadup();
                break;
            case J_B:
                if(strlen(guess) == 0) break;
                guess[strlen(guess)-1] = 0;
                render_guess();
                waitpadup();
                break;
            default:
            break;
        }

        wait_vbl_done();
    }
}

void main() {
    while(1) {
        run_wordle();
    }
}
