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
#include <stdio.h>
#include <stdint.h>
#include <rand.h>

#include <gbdk/incbin.h>
#include <gb/gbdecompress.h>

#include "word-db.h"

// Sprite Data
#include "../res/letter_cursor_tiles.h"

// Cursor sprite flipping flags to allow use of same tile for all 4 corners
const uint8_t sp_cursor_props[] = { 0x00, S_FLIPX, S_FLIPY, S_FLIPX | S_FLIPY };
const uint8_t sp_cursor_offset_x[] = { 0, 8, 0, 8 };
const uint8_t sp_cursor_offset_y[] = { 0, 0, 8, 8 };

#define SP_TILES_CURSOR_START 0
#define SP_TILES_CURSOR_LEN   1

#define SP_ID_CURSOR_START 0
#define SP_ID_CURSOR_LEN   4



// Map Data
//#include "../res/board_grid_tiles_gbcompress.h"
#include "../res/font_tiles_gbcompress.h"
// INCBIN(board_grid_map, "res/board_grid_map_gbcompress.bin")
// INCBIN_EXTERN(board_grid_map)
#include "../res/board_letter_1bpp_tiles.h"


// Blank tile, could be optimized
const uint8_t tile_blank[] = {0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00};


// #define MAP_BUF_SIZE (20 * 18)
// uint8_t map_scratch[MAP_BUF_SIZE];

// #define BOARD_GRID_X 2
// #define BOARD_GRID_Y 1
// #define BOARD_GRID_W 11
// #define BOARD_GRID_H 13

#define BG_TILES_BLANK_START 0
#define BG_TILES_BLANK_LEN   1
#define BG_TILES_FONT_START  (BG_TILES_BLANK_START + BG_TILES_BLANK_LEN)
#define BG_TILES_FONT_LEN    26
// #define BG_TILES_BOARD_START (BG_TILES_FONT_START + BG_TILES_FONT_LEN)
// #define BG_TILES_BOARD_LEN   15


// TODO: COUNT needs to be BOARD SIZE IN TILES (5 * 6 * 4)
#define BOARD_LETTERS_BYTES_PER_TILE 8 // 1bpp tiles = 8 bytes per 8x8 tile
#define BOARD_LETTERS_COUNT     27 // Last entry is blank space
#define BOARD_LETTERS_TILES_PER 4 // Last entry is blank space
// Empty space is last char in board font tile set
#define BOARD_LETTERS_SPACE_CHAR (BOARD_LETTERS_COUNT - 1)

uint8_t board_letters_decomp_buf[BOARD_LETTERS_COUNT * BOARD_LETTERS_TILES_PER * BOARD_LETTERS_BYTES_PER_TILE];

#define BOARD_GRID_X  2 // Start x,y in Tiles
#define BOARD_GRID_Y  1
#define BOARD_GRID_W  5 // Size in Tiles
#define BOARD_GRID_H  6
#define BOARD_TILE_W  2
#define BOARD_TILE_H  2
#define BOARD_GRID_TILES_PER_LETTER 4
#define BOARD_GRID_BYTES_PER_LETTER (BOARD_LETTERS_BYTES_PER_TILE * BOARD_GRID_TILES_PER_LETTER)
#define BOARD_GRID_TILES_PER_ROW    (BOARD_GRID_W * BOARD_GRID_TILES_PER_LETTER)
#define BOARD_GRID_TILE_W (BOARD_GRID_W * BOARD_TILE_W)
#define BOARD_GRID_TILE_H (BOARD_GRID_H * BOARD_TILE_H)

#define BOARD_TILE_X_START (BOARD_GRID_X)
#define BOARD_TILE_Y_START (BOARD_GRID_Y)
// #define BOARD_TILE_X_END   (BOARD_GRID_X + (BOARD_GRID_W * BOARD_TILE_W))
// #define BOARD_TILE_Y_END   (BOARD_GRID_Y + (BOARD_GRID_H * BOARD_TILE_H))

#define BG_TILES_BOARD_LETTERS_START (BG_TILES_FONT_START + BG_TILES_FONT_LEN)
#define BG_TILES_BOARD_LETTERS_LEN   (BOARD_GRID_W * BOARD_GRID_H * BOARD_GRID_TILES_PER_LETTER)

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


// Board Letter box colors
// #define LETTER_COLOR_NORMAL   set_1bpp_colors(DMG_BLACK, DMG_WHITE)     // Full contrast text
// #define LETTER_COLOR_CONTAINS set_1bpp_colors(DMG_BLACK, DMG_LITE_GRAY) // Filled in box text
// #define LETTER_COLOR_MATCHED  set_1bpp_colors(DMG_WHITE, DMG_BLACK)     // Inverted text
//
// Board Letter box colors
// Alt color scheme
#define LETTER_COLOR_NORMAL   set_1bpp_colors(DMG_LITE_GRAY, DMG_WHITE)  // Faded text
#define LETTER_COLOR_CONTAINS set_1bpp_colors(DMG_BLACK, DMG_WHITE)      // Full contrast text
#define LETTER_COLOR_MATCHED  set_1bpp_colors(DMG_BLACK, DMG_LITE_GRAY)  // Inverted text




uint8_t * print_vram_addr = NULL;

void print_gotoxy(uint8_t x, uint8_t y) {
    print_vram_addr = get_bkg_xy_addr(x,y);
}

#define BOARD_HIGHLIGHT_NO  false
#define BOARD_HIGHLIGHT_YES true

// gfx_print.c

// TODO: OPTIMZIE
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
}

// gfx_print.c

void print_str(char * txt) {

    uint8_t letter;

    while(*txt) {

        if(*txt >= 'A' && *txt <= 'Z'){
            letter = BG_TILES_FONT_START + (unsigned char)(*txt - 'A');
        } else if(*txt >= 'a' && *txt <= 'z') {
            letter = BG_TILES_FONT_START + (unsigned char)(*txt - 'a');
        // } else if(*txt >= '0' && *txt <= '9') {
        //     c = BG_TILES_FONT_NUMS_START + (unsigned char)(*txt - '0');
        } else {
            // Default is blank tile for Space or any other unknown chars
            letter = BG_TILES_BLANK_START;
        }

        set_vram_byte(print_vram_addr++, letter);
        txt++; // Next character
    }
}



void init_gfx_gameplay(void) {


    // Map Data

    // Clear screen
    fill_bkg_rect(0, 0, DEVICE_SCREEN_WIDTH, DEVICE_SCREEN_HEIGHT, BG_TILES_BLANK_START);

    // Load 2bpp blank tile
    gb_decompress_bkg_data((BG_TILES_BLANK_START), tile_blank);

    // Load 2bpp font tiles
    gb_decompress_bkg_data((BG_TILES_FONT_START), font_tiles);

    // // Load 2bpp board tiles
    // gb_decompress_bkg_data((BG_TILES_BOARD_START), board_grid_tiles);

    // // Decompress board map and render it to screen
    // gb_decompress(board_grid_map, map_scratch);
    // set_bkg_based_tiles(BOARD_GRID_X, BOARD_GRID_Y, BOARD_GRID_W, BOARD_GRID_H,
    //                     map_scratch, BG_TILES_BOARD_START);

    // set_1bpp_colors(DMG_WHITE, DMG_BLACK);
    // set_1bpp_colors(DMG_DARK_GRAY, DMG_LITE_GRAY);
    // set_bkg_1bpp_data(BG_TILES_BOARD_LETTERS_START, BG_TILES_BOARD_LETTERS_LEN, board_letter_tiles);

    // Decompress board letter tiles
    gb_decompress(board_letter_tiles, board_letters_decomp_buf);

    // // Test: render them to screen
    // set_1bpp_colors(DMG_BLACK, DMG_WHITE);
    // set_1bpp_colors(DMG_BLACK, DMG_WHITE);
    // set_1bpp_colors(DMG_BLACK, DMG_WHITE);
    // // set_1bpp_colors(DMG_DARK_GRAY, DMG_LITE_GRAY);
    // set_bkg_1bpp_data(BG_TILES_BOARD_LETTERS_START, BG_TILES_BOARD_LETTERS_LEN, board_letters_decomp_buf);

    // TODO: this + map takes up about 300 bytes, optimize further? seems like setup call for this is what costs the most
    // Set up Board Letter map in VRAM
    // (direct addressable for rewriting letters via changing tile contents)
    set_bkg_based_tiles(BOARD_TILE_X_START, BOARD_TILE_Y_START,
                        BOARD_GRID_TILE_W, BOARD_GRID_TILE_H,
                        board_map,
                        BG_TILES_BOARD_LETTERS_START);




    print_gotoxy(2,0);
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

    SHOW_BKG;
    SHOW_SPRITES;
    DISPLAY_ON;
}





#define MAX_GUESSES 6

void set_box_color_for_letter(char *word, int position, char letter);
void set_letter_color_for_letter(char *word, int position, char letter);

const char *kb[3] = {
"Q W E R T Y U I O P",
" A S D F G H J K L",
"  Z X C V B N M"};

int kb_coords[3] = {
    10,
    9,
    7
};

int kb_offsets[3] = {
    0,
    1,
    2
};

int kb_x = 0;
int kb_y = 0;
int guess_nr;
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

    // Draw letter into VRAM address for resired board tile
    // From offset into 1bpp letter source of 4x4 metatiles
    set_bkg_1bpp_data(BG_TILES_BOARD_LETTERS_START
                        + (col * BOARD_GRID_TILES_PER_LETTER)
                        + (row * BOARD_GRID_TILES_PER_ROW),
                      BOARD_GRID_TILES_PER_LETTER,
                      board_letters_decomp_buf + (letter * BOARD_GRID_BYTES_PER_LETTER));
}


const uint8_t empty_word_buf[] = {' ', ' ', ' ', ' ', ' '};

// Render a word at * p_guess onto the board
void board_draw_word(uint8_t row, uint8_t * p_guess, bool do_highlight) {

    if (p_guess == NULL)
        p_guess = empty_word_buf;

do_highlight = true;
    // col maps to the individual letters in the word/guess
    for (uint8_t col = 0; col < BOARD_GRID_W; col ++) {
        if (do_highlight)
            set_letter_color_for_letter(word, col, p_guess[col]);
        board_draw_letter(row, col, p_guess[col]);
    }
}

void draw_word_rect(int x, int y, char *guess) {
/*
    int gx = x/8;
    int gy = y/8;
    x -= 3;
    y -= 4;
    for(int i=0; i < 5; i++) {
        if(guess) {
            char letter = guess[i];
            set_box_color_for_letter(word, i, letter);
            box(x, y, x+14, y+14, M_FILL);
            set_letter_color_for_letter(word, i, letter);
            gotogxy(gx, gy);
            wrtchr(letter);
            gx += 2;
        } else {
            color(BLACK, WHITE, M_NOFILL);
            box(x, y, x+14, y+14, M_NOFILL);
        }

        x += 16;
    }
*/
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


void set_letter_color_for_letter(char *word, int position, char letter) {

    if(word[position] == letter) {
        LETTER_COLOR_MATCHED;

    } else if(contains(word, letter)) {
        LETTER_COLOR_CONTAINS;

    } else {
        LETTER_COLOR_NORMAL;
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


void set_color_for_letter(char letter) {
    if(letter == ' ')
    {
        set_1bpp_colors(DMG_BLACK, DMG_WHITE);
    }
    else if (contains(guessed_wrong, letter))
    {
        set_1bpp_colors(DMG_LITE_GRAY, DMG_WHITE);
    }
    else if(contains(guessed_position, letter))
    {
        set_1bpp_colors(DMG_DARK_GRAY, DMG_WHITE);
    }
    else if(contains(guessed_correct, letter))
    {
        set_1bpp_colors(DMG_DARK_GRAY, DMG_WHITE);
    }
    else {
        set_1bpp_colors(DMG_BLACK, DMG_WHITE);
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

void draw_keyboard(int x, int y) {

    for(int i=0; i < 3; i++) {
        print_gotoxy(x, y + i);
        int kbl = strlen(kb[i]);
        for(int j=0; j < kbl; j++) {
            char letter = kb[i][j];
            set_color_for_letter(letter);
            print_char(letter);
        }
    }

/*
    for(int i=0; i < 3; i++) {
        gotogxy(x, y + i);
        int kbl = strlen(kb[i]);
        for(int j=0; j < kbl; j++) {
            char letter = kb[i][j];
            set_color_for_letter(letter);
            wrtchr(letter);
        }
    }
*/
}

int kb_vert_offset = 14;

// Move the cursor to highlight a key
void highlight_key() {

    uint8_t x = (kb_x * 16) + (kb_offsets[kb_y] * 8) + DEVICE_SPRITE_PX_OFFSET_X;
    uint8_t y = ((kb_vert_offset + kb_y) * 8) + DEVICE_SPRITE_PX_OFFSET_Y;

    for (uint8_t i = 0; i < SP_ID_CURSOR_LEN; i++) {
        move_sprite(SP_ID_CURSOR_START + i, x + sp_cursor_offset_x[i], y + sp_cursor_offset_y[i] );
    }
/*
    int x = (kb_x * 16) + (kb_offsets[kb_y] * 8);
    int y = (kb_vert_offset + kb_y) * 8;
    color(BLACK, WHITE, M_NOFILL);
    box(x, y-1, x+8, y+7, M_NOFILL);
*/
}

void dehighlight_key() {
/*
    int x = (kb_x * 16) + (kb_offsets[kb_y] * 8);
    int y = (kb_vert_offset + kb_y) * 8;
    color(WHITE, WHITE, M_NOFILL);
    box(x, y-1, x+8, y+7, M_NOFILL);


    int gx = (kb_x * 2) + (kb_offsets[kb_y]);
    int gy = (kb_vert_offset + kb_y);
    gotogxy(gx, gy);
    char letter = kb[kb_y][kb_offsets[kb_y]+ (kb_x*2)];
    set_color_for_letter(letter);
    wrtchr(letter);
*/
}

char getletter() {
    return kb[kb_y][kb_offsets[kb_y] + (kb_x*2)];
}


void render_guess() {

    LETTER_COLOR_NORMAL;
    board_draw_word(guess_nr, guess, BOARD_HIGHLIGHT_NO); // guess_nr is row

/*    // first box is at 5, 2
    int line = 2 + (guess_nr * 2);
    int x = 5;
    for(int i=0; i < 5; i++) {
        color(BLACK, WHITE, M_NOFILL);
        gotogxy(x, line);
        if(guess[i] != 0) {
            wrtchr(guess[i]);
        } else {
            wrtchr(' ');
        }

        x += 2;
    }
*/
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

/*
    for(int i=0; i < MAX_GUESSES; i++) {
        char *g = NULL;
        if(i < guess_nr) {
            g = guesses[i];
        }
        draw_word_rect(40, 16+(i*16), g);
    }
*/
}


void show_win() {
/*
    color(BLACK, BLACK, M_FILL);
    box(0, 0, SCREENWIDTH, SCREENHEIGHT, M_FILL);
    gotogxy(0, 8);
    color(WHITE, BLACK, M_NOFILL);
    gprint("     You won!!!");
    gotogxy(0, 9);
    gprintf("   %d/6 - Congrats!", guess_nr);
*/
    waitpad(J_START | J_A);
    reset();
}

/*void show_loose() {
    // cls();
    color(BLACK, BLACK, M_FILL);
    box(0, 0, SCREENWIDTH, SCREENHEIGHT, M_FILL);
    gotogxy(0, 8);
    color(WHITE, BLACK, M_NOFILL);
    gprint(" You lost. Sorry!");

    waitpad(J_START | J_A);
    reset();
}
*/
void show_lose(char *correct_word) {
/*    // cls();
    char ans[12] = " Answer: ";
    color(BLACK, BLACK, M_FILL);
    box(0, 0, 160, 144, M_FILL);
    gotogxy(0, 8);
    color(WHITE, BLACK, M_NOFILL);
    gprint(" You lost. Sorry!");
    gotogxy(0, 10);
    color(WHITE, BLACK, M_NOFILL);
    gprint(strncat(ans, correct_word, 5));
*/
    waitpad(J_START | J_A);
    reset();
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

    // Draws initial empty board
    for(int i=0; i < MAX_GUESSES; i++) {
        board_draw_word(i, NULL, BOARD_HIGHLIGHT_NO);
    }

/*
    for(int i=0; i < MAX_GUESSES; i++) {
        draw_word_rect(40, 16+(i*16), NULL);
    }
*/
/*
    gotogxy(2, 0);
    gprint("GameBoy  WORDLE");
*/    draw_keyboard(0, kb_vert_offset);

/*
    color(LTGREY, WHITE, M_NOFILL);
*/
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
                dehighlight_key();
                kb_x += 1;
                if(kb_x >= kb_coords[kb_y]) {
                    kb_x = 0;
                }
                highlight_key();
                waitpadup();
                break;
            case J_LEFT:
                dehighlight_key();
                kb_x -= 1;
                if(kb_x < 0) {
                    kb_x = kb_coords[kb_y] - 1;
                }
                highlight_key();
                waitpadup();
                break;
            case J_UP:
                dehighlight_key();
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
                dehighlight_key();
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
                draw_keyboard(0, kb_vert_offset);
                highlight_key();
                if(strcmp(word, guess) == 0) {
                    show_win();
                    return;
                    break;
                }
                if(guess_nr == MAX_GUESSES) {
                    // show_loose();
                    show_lose(word);
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
