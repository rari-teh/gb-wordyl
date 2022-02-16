// window.h

#ifndef _WINDOW_H
#define _WINDOW_H

void win_dialog_draw(void);
void win_dialog_show_message(uint8_t win_y_moveto, uint8_t * str_1, uint8_t * str_2);

#define DIALOG_WON_MESSAGE_WIN_Y  (DEVICE_SCREEN_PX_HEIGHT - (8 * 4.5)) // 4.5 rows tall
#define DIALOG_LOSE_MESSAGE_WIN_Y  (DIALOG_WON_MESSAGE_WIN_Y)
#define DIALOG_GAME_INFO_Y         (DIALOG_WON_MESSAGE_WIN_Y)

#endif
