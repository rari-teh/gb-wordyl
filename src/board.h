// board.h

#ifndef _BOARD_H
#define _BOARD_H


// Faster flip speeds look ok on all hardware except stock CGB LCD
// which seems to drop some frames?
// #define ENABLE_BOARD_SLOW_FLIP

#define BOARD_TILE_FLIP_SLOW    2u  // See ENABLE_BOARD_SLOW_FLIP
#define BOARD_TILE_FLIP_FAST    1u
#define BOARD_TILE_FLIP_NONE    0u

extern uint8_t g_board_tile_flip_speed;

// Turns board tile flipping on/off and sets speed in board_draw_letter()
// Use with BOARD_TILE_FLIP_*
#define BOARD_SET_FLIP_SPEED(speed) g_board_tile_flip_speed = (speed)

void board_hide_cursor(void);
void board_update_cursor(void);

void board_redraw_clean(void);
void board_render_guess_letter(uint8_t col);
void board_add_guess_letter(void);
void board_remove_guess_letter(void);

void board_draw_letter_bits(uint8_t row, uint8_t col, uint8_t letter);
void board_draw_tile_flip_anim(uint8_t row, uint8_t col);
void board_draw_letter(uint8_t row, uint8_t col, uint8_t letter, bool do_highlight);
void board_draw_word(uint8_t row, uint8_t * p_guess, bool do_highlight);
void board_set_color_for_letter(uint8_t row, uint8_t col, uint8_t do_highlight);

void board_initgfx(void);

#endif
