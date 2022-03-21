// window.c

#include <gbdk/platform.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>


#include "common.h"
#include "input.h"
#include "gfx_common.h"
#include "gameboy_color.h"

#include "window.h"
#include "lang_text.h"


// Draw dialog box outline on the window
void win_dialog_draw(void) {

    set_win_based_tiles(0,0, DEVICE_SCREEN_WIDTH, DEVICE_SCREEN_HEIGHT, map_decomp_buf, BG_TILES_INTRO_DIALOG_START);
}

#define DIALOG_CONFIRM_BUTTON  (J_UP)

// Prompts the user to confirm based on: str_message
// Returns TRUE if confirmed
bool win_confirm_dialog(char * str_message) {

    return (DIALOG_CONFIRM_BUTTON == win_dialog_show_message(CONFIRM_DIALOG_WIN_Y, __CONFIRM_DIALOG_STR, str_message));
}


// Draw dialog box outline on the window
uint8_t win_dialog_show_message(uint8_t win_y_moveto, uint8_t * str_1, uint8_t * str_2) {
    uint8_t win_y_save = WY_REG;
    uint8_t scroll_amt;
    uint8_t ret_keys_ticked;

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

    waitpadticked_lowcpu(J_ANY_KEY);
    ret_keys_ticked = KEYS_GET_TICKED();

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

    return ret_keys_ticked;
}


