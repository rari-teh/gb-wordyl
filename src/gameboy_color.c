// gameboy_color.c

#include <gbdk/platform.h>

#include "common.h"
#include "gfx_common.h"
#include "gameboy_color.h"

const palette_color_t cgb_palettes[] = {
                               RGB8(255u,255u,255u), RGB8(170u,170u,170u),RGB8(85u,85u,85u),RGB8(0u,0u,0u),
                               RGB8(255u,243u, 86u), RGB8(170u,170u,170u),RGB8(85u,85u,85u),RGB8(0u,0u,0u), // col 0 = yellow
                               RGB8(130u,255u,130u), RGB8(170u,170u,170u),RGB8(85u,85u,85u),RGB8(0u,0u,0u), // col 0 = green
                               RGB8(168u,240u,255u), RGB8(170u,170u,170u),RGB8(85u,85u,85u),RGB8(0u,0u,0u), // col 0 = blue
                               RGB8(255u,150u,150u), RGB8(170u,170u,170u),RGB8(85u,85u,85u),RGB8(0u,0u,0u), // col 0 = red
                          };

// Initialize CGB palettes
void cgb_set_palettes(void) {
    set_bkg_palette(0, ARRAY_LEN(cgb_palettes) / 4, cgb_palettes);
    set_sprite_palette(0, ARRAY_LEN(cgb_palettes) / 4, cgb_palettes);
}


// Use CGB 2x mode if available and set up CGB palettes
void cgb_check_and_init(void) {

    if (IS_CGB) {
        cpu_fast();
        cgb_set_palettes();
    }
}


// TODO: board.c

// Set up colors for board area
void board_initgfx_cgb(void) {

    // Select CGB attribute tile mode for bkg map access
    VBK_REG = VBKF_BKG_ATTRIB;

    // Fill screen top region under baord with blue
    fill_bkg_rect(0,0, DEVICE_SCREEN_WIDTH,
                  BOARD_GRID_Y + BOARD_GRID_TILE_H,
                  CGB_PAL_SCREEN_BKG);

    // Return to normal tile BKG mode
    VBK_REG = VBKF_BKG_TILES;
}


// Set the CGB color for a board letter square
void board_fill_letter_cgb_pal(uint8_t row, uint8_t col, uint8_t palnum) {

    // Select CGB attribute tile mode for bkg map access
    VBK_REG = VBKF_BKG_ATTRIB;

    // Fill region with requested CGB palette attribute
    fill_bkg_rect(BOARD_GRID_X + (col * BOARD_TILE_W),
                  BOARD_GRID_Y + (row * BOARD_TILE_H),
                  BOARD_TILE_W, BOARD_TILE_H,
                  palnum & CGB_PAL_MASK);

    // Return to normal tile BKG mode
    VBK_REG = VBKF_BKG_TILES;
}


