// window.h

#ifndef _WINDOW_H
#define _WINDOW_H

void win_dialog_draw(void);
uint8_t win_dialog_show_message(uint8_t win_y_moveto, uint8_t * str_1, uint8_t * str_2);

#define PX_PER_TILE        8
#define DIALOG_BORDER_SIZE (1 * PX_PER_TILE)

#define DIALOG_WON_MESSAGE_WIN_Y    (DEVICE_SCREEN_PX_HEIGHT - ((4 * PX_PER_TILE) + DIALOG_BORDER_SIZE))
#define DIALOG_LOSE_MESSAGE_WIN_Y   (DIALOG_WON_MESSAGE_WIN_Y)
#define DIALOG_GAME_INFO_Y          (DIALOG_WON_MESSAGE_WIN_Y)
#define DIALOG_STATS_MESSAGE_WIN_Y  (DEVICE_SCREEN_PX_HEIGHT - ((10 * PX_PER_TILE) + DIALOG_BORDER_SIZE))

#define DIALOG_MENU_WIN_Y  (DEVICE_SCREEN_PX_HEIGHT - ((9 * PX_PER_TILE) + DIALOG_BORDER_SIZE))

#define DIALOG_INFO_WIN_Y           (DIALOG_WON_MESSAGE_WIN_Y)

#define DIALOG_WELCOME_WIN_Y  (DEVICE_SCREEN_PX_HEIGHT - ((11 * PX_PER_TILE) + DIALOG_BORDER_SIZE))

#endif
