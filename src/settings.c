// common.c

#include <gbdk/platform.h>
#include <stdint.h>
#include <stdbool.h>

#include "stats.h"
#include "common.h"
#include "gfx_common.h"

#include <lang_text.h>
#include <cartsave.h>


// Defaults for power-on or save not present
inline void options_reset() {
    game_settings.opt_hard_mode_enabled = false;
    game_settings.opt_autofill_enabled = false;
        // Not implemented
        game_settings.opt_sound_fx_enabled = true;
        game_settings.opt_music_type = 1;
        game_settings.opt_tile_flip_enabled = true;
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
