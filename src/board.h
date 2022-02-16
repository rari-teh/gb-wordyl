// board.h

#ifndef _BOARD_H
#define _BOARD_H

void board_redraw_clean(void);
void board_render_guess_letter(uint8_t col);
void board_add_guess_letter(void);
void board_remove_guess_letter(void);

void board_draw_letter(uint8_t row, uint8_t col, uint8_t letter);
void board_draw_word(uint8_t row, uint8_t * p_guess, bool do_highlight);
void board_set_color_for_letter(uint8_t row, uint8_t col, uint8_t do_highlight);

void board_initgfx(void);

#endif
