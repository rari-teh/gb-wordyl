// window.h

#ifndef _WINDOW_H
#define _WINDOW_H

void win_dialog_draw(void);
bool win_confirm_dialog(char * str_message);
uint8_t win_dialog_show_message(uint8_t win_y_moveto, uint8_t * str_1, uint8_t * str_2);

#define PX_PER_TILE        8
#define DIALOG_BORDER_SIZE (1 * PX_PER_TILE)

// Adds one extra line at bottom for padding
// MAXIMUM input value is 16
#define DIALOG_TEXT_HEIGHT_LINES(LINES)  (DEVICE_SCREEN_PX_HEIGHT - (((LINES + 1) * PX_PER_TILE) + DIALOG_BORDER_SIZE))

#endif
