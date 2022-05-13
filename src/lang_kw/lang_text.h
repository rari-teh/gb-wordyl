// lang_text.h

#ifndef _LANG_TEXT_H
#define _LANG_TEXT_H

// == This has default strings
#include "../lang_text_defaults.inc"


// == #define overrides for language strings go here

// Kernewek (Cornish Language): Translation sjd_sjd_
// Meur ras dhe Niwlen Ster, Kamm Cam ha Dyski Kernowek Discord

#undef INTRO_DIALOG_WIN_Y
#define INTRO_DIALOG_WIN_Y DIALOG_TEXT_HEIGHT_LINES(14)
#undef __INTRO_MESSAGE_STR
#define __INTRO_MESSAGE_STR \
/*  |------------------| Available space (18 chars wide) */\
    "DYNNARGH DHE\n" \
    "WORDYL\n"\
    "\n" \
    "YETH ... " STR(LANG_CODE) "\n" \
    "  A..Z  HEPKEN\n" \
    "ROL Y'N GWARI:\n" \
    "  3 x SELECT\n" \
    "RESELL BORDH:\n" \
    "  SELECT + B A\n" \
    "OMLENWEL:\n" \
    "  SELECT + START\n" \
    "\n" \
    "GWASK BOTON\n" \
    "HA DALLETH"


#undef MENU_Y_EXIT
#undef MENU_Y_HARD_MODE
#undef MENU_Y_AUTO_FILL
#undef MENU_Y_SOUND_FX
#undef MENU_Y_SHOW_STATS
#undef MENU_Y_FORFEIT_ROUND
#undef MENU_Y_RESET_STATS
// "^" char gets replaced with a UI checkboxes reflecting option on/off state
// Leading space on each line is room for Cursor
// MAX Height is 16
#undef __OPTIONS_MENU_STR
#define __OPTIONS_MENU_STR \
/*  |------------------| Available space (18 chars wide) */\
    " ... DEWISYOW ...\n" \
    "\n" \
    " KWYTYA ROL\n" \
    "\n" \
    " NIVEL KALES ... ^\n" \
    " OMLENWEL ...... ^\n" \
    " SOUND FX ...... ^\n" \
    "\n" \
    " DISKWEDHES STATS\n" \
    " KWYTYA TRO\n" \
    " DASSETTYA STATS"

// These need to exactly match the menu order & formating above
// Line 0 is the first line of text
// menu header               0
// space                     1
#define MENU_Y_EXIT          2
// space                     3
#define MENU_Y_HARD_MODE     4
#define MENU_Y_AUTO_FILL     5
#define MENU_Y_SOUND_FX      6
// space                     7
#define MENU_Y_SHOW_STATS    8
#define MENU_Y_FORFEIT_ROUND 9
#define MENU_Y_RESET_STATS   10


#define CONFIRM_DIALOG_WIN_Y  DIALOG_TEXT_HEIGHT_LINES(6)
#undef __CONFIRM_DIALOG_STR
#define __CONFIRM_DIALOG_STR \
/*  |------------------| Available space (18 chars wide) */ \
    "AFYDHYA:         ^\n" \
    "HEDHI:  BOTON ARAL\n"\
    "..................\n" \
    "\n" \

#undef __CONFIRM_FORFEIT_STR
#define __CONFIRM_FORFEIT_STR     "KWYTYA TRO?"

#undef __CONFIRM_STATS_RESET_STR
#define __CONFIRM_STATS_RESET_STR "DASSETTYA STATS?"

#undef __AUTOFILL_ON__STR
#undef __AUTOFILL_OFF__STR
#define __AUTOFILL_ON__STR "OMLENWEL: YN FYW"
#define __AUTOFILL_OFF__STR "OMLENWEL: YN FAROW"


// Display hard mode indicator at top of screen
#undef OPT_HARDMODE_DISPLAY_X
#undef OPT_HARDMODE_DISPLAY_Y
#define OPT_HARDMODE_DISPLAY_X 5
#define OPT_HARDMODE_DISPLAY_Y 0

#undef __STR_HARD_MODE_DISPLAY
#undef __STR_HARD_MODE_HIDE
#define __STR_HARD_MODE_DISPLAY "KALES"
#define __STR_HARD_MODE_HIDE    "       "


#undef __MESSAGE_HARD_MODE_CANT_CHANGE_STR
#define __MESSAGE_HARD_MODE_CANT_CHANGE_STR \
/*  |------------------| Available space (18 chars wide) */\
    "N.KALES:RES YW BOS\n" \
    "AN CHANJ GWRYS\n" \
    "ORTH PENN AN DRO"


// Avoid increasing this popup height so it doesn't obscure the board
#undef __MESSAGE_HARD_MODE_GUESS_NOT_VALID_STR
#define __MESSAGE_HARD_MODE_GUESS_NOT_VALID_STR \
/*  |------------------| Available space (18 chars wide) */\
    "NIVEL KALES: RES \n" \
    "YW USYA PUB HYNT\n" \
    "Y'N DISMYK"


// Avoid increasing this popup height so it doesn't obscure the board
#undef __MESSAGE_LOSE_STR
#define __MESSAGE_LOSE_STR \
/*  |------------------| Available space (18 chars wide) */\
    "TY A WRUG FYLLEL!\n" \
    "\n" \
    "GORTHYP: "    // <-- Needs 5 chars left for answer word!


// NOTE! ^ gets replaced with guess 5 letter word num
//
// Avoid increasing this popup height so it doesn't obscure the board
#undef __MESSAGE_GAME_WON_STR
#define __MESSAGE_GAME_WON_STR \
/*  |------------------| Available space (18 chars wide) */\
    "TY A WRUG GWAYNYA!\n" \
    "\n" \
    "DISMYGOW: ^"


// Avoid increasing this popup height so it doesn't obscure the board
#undef __MESSAGE_WORD_TOO_SHORT_STR
#define __MESSAGE_WORD_TOO_SHORT_STR \
/*  |------------------| Available space (18 chars wide) */\
    "RE VERR YW AN GER!\n" \
    "RES YW BOS 5\n" \
    "LYTHEREN YNNO"


// Avoid increasing this popup height so it doesn't obscure the board
#undef __MESSAGE_WORD_NOT_IN_DICT_STR
#define __MESSAGE_WORD_NOT_IN_DICT_STR \
/*  |------------------| Available space (18 chars wide) */\
    "GER ANKOTH\n"


#undef __MESSAGE_STATS_RESET_STR
#define __MESSAGE_STATS_RESET_STR \
/*  |------------------| Available space (18 chars wide) */\
    "DASSETTYA STATS!"


#undef __STATS_TEMPLATE_STR
#define __STATS_TEMPLATE_STR \
/*  |------------------| Available space (18 chars wide) */\
    "GWARIOW\n" \
    "  YN SOMM... ____^\n" \
    " KADON   ... ____^\n" \
    " GWAYNYOW... ____^\n" \
    " FALLOW  ... ____^\n" \
    "LESRANNANS DISMYK\n" \
    " LYHA    ... ____^\n" \
    " MOYHA   ... ____^\n" \
    " KRESEK  ... ____^\n"   // <-- All "____^" get replaced by numbers

// Should be no more than 18 chars
#undef __INTRO_PRESS_START_STR
#define __INTRO_PRESS_START_STR "GWASK START"


// X, Y controls upper-left corner coordinates
#undef __INTRO_CREDITS_STR
#define __INTRO_CREDITS_STR \
/*  |--------------------| */\
        "MEUR RAS DHE :\n" \
        "TOXA\n" \
        "DAEO\n" \
        "ARPRUSS\n" \
        "COFFEE 'VALEN' BAT\n" \
        "STACKSMASHING\n" \
        "SJD\n" \
        "\n" \
        "GBDK2020\n" \
        "EMULICIOUS\n" \
        "FERRANTE CRAFTS"


#endif
