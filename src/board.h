// board.h

#ifndef _BOARD_H
#define _BOARD_H

void board_redraw(void);
void board_render_guess(void);

void board_draw_letter(uint8_t row, uint8_t col, uint8_t letter);
void board_draw_word(uint8_t row, uint8_t * p_guess, bool do_highlight);
void board_set_color_for_letter(uint8_t row, uint8_t col, char *word, char letter, uint8_t do_highlight);

void board_initgfx(void);

#endif
