// settings.c

#include <gbdk/platform.h>
#include <stdint.h>
#include <stdbool.h>

#include "stats.h"
#include "common.h"
#include "gfx_common.h"
#include "sfx_common.h"

#include "window.h"

#include <lang_text.h>
#include <cartsave.h>


// Defaults for power-on or save not present
void options_reset() {
    game_settings.opt_hard_mode_enabled = false;
    game_settings.opt_autofill_enabled = false;
        // Not implemented
        game_settings.opt_sound_fx_enabled = true;
        game_settings.opt_music_type = 1;
        game_settings.opt_tile_flip_enabled = true;

    // For relevant carts, save the reset stats
    #if defined(CART_31k_1kflash) || defined(CART_mbc5)
        cartsave_save_data();
    #endif
}


// Initialize settings and stats on power-up, try to load values from cart sram/flash
void settings_load(void) {

    #if defined(CART_31k_1kflash) || defined(CART_mbc5)
        // Check signature, reset stats and notify if signature failed
        // It is expected to fail on first power-up
        cartsave_restore_data();
        // No CRC calc on data to allow growing record struct without resetting it

        if ((game_settings.save_check0 != STATS_SIG_CHECK_0) || (game_settings.save_check1 != STATS_SIG_CHECK_1)) {
            stats_reset();
            options_reset();
            win_dialog_show_message(STATS_RESET_DIALOG_WIN_Y, __MESSAGE_STATS_RESET_STR ,NULL);
        }
    #else
        // (bare 32k) Saveless cart just resets stats instead of loading
        stats_reset();
        options_reset();
    #endif

    // Update display of settings (just hard mode for now)
    opt_hardmode_display();
}


// General handler for inverting a bool settings option
// Called from the popup settings menu
void setting_bool_handle_change(bool * p_bool) {

    if (*p_bool == true) *p_bool = false;
    else *p_bool = true;

    play_sfx(SFX_MENU_ACTION_ACKNOWLEDGE);

    // For relevant carts, save the reset stats
    #if defined(CART_31k_1kflash) || defined(CART_mbc5)
        cartsave_save_data();
    #endif
}


void hardmode_handle_change(void) {

    if (guess_num > 0)
        win_dialog_show_message(HARD_MODE_CANT_CHANGE_WIN_Y, __MESSAGE_HARD_MODE_CANT_CHANGE_STR, NULL);
    else {
        setting_bool_handle_change(&game_settings.opt_hard_mode_enabled);
        opt_hardmode_display();
    }
}


void autofill_handle_change(void) {

    setting_bool_handle_change(&game_settings.opt_autofill_enabled);

    win_dialog_show_message(AUTOFILL_INFO_WIN_Y,
                            (game_settings.opt_autofill_enabled ? __AUTOFILL_ON__STR : __AUTOFILL_OFF__STR), NULL);
}


void soundfx_handle_change(void) {
    setting_bool_handle_change(&game_settings.opt_sound_fx_enabled);
}

