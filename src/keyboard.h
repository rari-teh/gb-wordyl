// keyboard.h

#ifndef _KEYBOARD_H
#define _KEYBOARD_H

extern const char *kb[3];
extern int8_t kb_coords[3];
extern int8_t kb_offsets[3];

extern int8_t kb_x;
extern int8_t kb_y;

extern const uint8_t sp_cursor_props[];
extern const uint8_t sp_cursor_offset_x[];
extern const uint8_t sp_cursor_offset_y[];


inline char keyboard_get_letter(void);

void keyboard_fill_letter_cgb_pal(uint8_t row, uint8_t col, uint8_t palnum);
void keyboard_set_color_for_letter(uint8_t row, uint8_t col, uint8_t match_type);
inline void keyboard_set_default_color_letter(uint8_t row, uint8_t col);

void keyboard_update_from_guess(void);
void keyboard_redraw_clean(void);
void keyboard_draw_map(void);

void keyboard_update_cursor(void);

#endif
