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


// Draw dialog box outline on the window
void win_dialog_draw(void) {

    // Clear window area
    fill_win_rect(0, 0, DEVICE_SCREEN_WIDTH, DEVICE_SCREEN_HEIGHT, BG_TILES_BLANK_START );

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

    waitpadticked_lowcpu(J_ANY_KEY, NULL);

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


