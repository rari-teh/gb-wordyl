// lang_text.h

#ifndef _LANG_TEXT_H
#define _LANG_TEXT_H

// == This has default strings
#include "../lang_text_defaults.inc"


// == #define overrides for language strings go here

// Dutch (edit by FerrantePescara)

#undef INTRO_DIALOG_WIN_Y
#define INTRO_DIALOG_WIN_Y DIALOG_TEXT_HEIGHT_LINES(16)
#undef __INTRO_MESSAGE_STR
#define __INTRO_MESSAGE_STR \
/*  |------------------| Available space (18 chars wide) */\
    "WELKOM BIJ WORDYL \n" \
    "\n" \
    "TAAL .......... " STR(LANG_CODE) "\n" \
    "ALLEEN A..Z ZONDER\n" \
    "  SPECIALE TEKENS \n" \
    "\n" \
    "OPTIEMENU:        \n" \
    "  3 x SELECT\n" \
    "\n" \
    "CURSOR:\n" \
    "  SELECT + B A\n" \
    "\n" \
    "AUTO AANVULLEN:\n" \
    "  SELECT + START\n" \
    "\n" \
    "DRUK OP EEN KNOP"


#undef __OPTIONS_MENU_STR
#define __OPTIONS_MENU_STR \
/*  |------------------| Available space (18 chars wide) */\
    "OPTIES\n" \
    "\n" \
    "TOON STATS:      A\n" \
    "MOEILIJKE MODUS  >\n" \
    "AUTO AANVULLEN:  /\n" \
    "\n" \
    "RONDE OPGEVEN:   ^\n" \
    "STATS RESETTEN:  <\n" \
    "\n" \
    "DRUK OP EEN KNOP  "


#define CONFIRM_DIALOG_WIN_Y  DIALOG_TEXT_HEIGHT_LINES(6)
#undef __CONFIRM_DIALOG_STR
#define __CONFIRM_DIALOG_STR \
/*  |------------------| Available space (18 chars wide) */ \
    "DOORGAAN:^\n" \
    "STOP: ANDERE KNOP\n"\
    "..................\n" \
    "\n" \

#undef __CONFIRM_FORFEIT_STR
#define __CONFIRM_FORFEIT_STR     "RONDE OPGEVEN?"

#undef __CONFIRM_STATS_RESET_STR
#define __CONFIRM_STATS_RESET_STR "STATS RESETTEN?"

#undef __AUTOFILL_ON__STR
#undef __AUTOFILL_OFF__STR
#define __AUTOFILL_ON__STR "AUTO AANVULLEN:AAN"
#define __AUTOFILL_OFF__STR "AUTO AANVULLEN:UIT"


// Display hard mode indicator at top of screen
#undef OPT_HARDMODE_DISPLAY_X
#undef OPT_HARDMODE_DISPLAY_Y
#define OPT_HARDMODE_DISPLAY_X 5
#define OPT_HARDMODE_DISPLAY_Y 0

#undef __STR_HARD_MODE_DISPLAY
#undef __STR_HARD_MODE_HIDE
#define __STR_HARD_MODE_DISPLAY "MOEILIJK"
#define __STR_HARD_MODE_HIDE    "        "


#undef __MESSAGE_HARD_MODE_CANT_CHANGE_STR
#define __MESSAGE_HARD_MODE_CANT_CHANGE_STR \
/*  |------------------| Available space (18 chars wide) */\
    "MOEILIJKE MODUS:  \n" \
    "MAG ALLEEN AAN HET\n" \
    "BEGIN WIJZIGEN    "


#undef __MESSAGE_HARD_MODE_GUESS_NOT_VALID_STR
#define __MESSAGE_HARD_MODE_GUESS_NOT_VALID_STR \
/*  |------------------| Available space (18 chars wide) */\
    "MOEILIJKE MODUS:  \n" \
    "ALLE AANWIJZINGEN \n" \
    "GEBRUIKEN VEREIST "


#undef __MESSAGE_LOSE_STR
#define __MESSAGE_LOSE_STR \
/*  |------------------| Available space (18 chars wide) */\
    "JE HEBT VERLOREN! \n" \
    "\n" \
    "ANTWOORD: "    // <-- Needs 5 chars left for answer word


// NOTE! ^ gets replaced with guess 5 letter word num
//
// Intentionally not const! ^ gets replaced with guess num
#undef __MESSAGE_GAME_WON_STR
#define __MESSAGE_GAME_WON_STR \
/*  |------------------| Available space (18 chars wide) */\
    "JE HEBT GEWONNEN! \n" \
    "\n" \
    "GOK ^ VAN 6"


#undef __MESSAGE_WORD_TOO_SHORT_STR
#define __MESSAGE_WORD_TOO_SHORT_STR \
/*  |------------------| Available space (18 chars wide) */\
    "WOORD IS TE KORT! \n" \
    "\n" \
    "5 LETTERS NODIG"

#undef __MESSAGE_WORD_NOT_IN_DICT_STR
#define __MESSAGE_WORD_NOT_IN_DICT_STR \
/*  |------------------| Available space (18 chars wide) */\
    "WOORD ZIT NIET IN\n" \
    "\n" \
    "HET WOORDENBOEK!"


#undef __MESSAGE_STATS_RESET_STR
#define __MESSAGE_STATS_RESET_STR \
/*  |------------------| Available space (18 chars wide) */\
    "STATS GERESET!"


#undef __STATS_TEMPLATE_STR
#define __STATS_TEMPLATE_STR \
/*  |------------------| Available space (18 chars wide) */\
    " GESPEELD  ____^\n" \
    " REEKS     ____^\n" \
    " GEWONNEN  ____^\n" \
    " VERLOREN  ____^\n" \
    "\n" \
    "GOKKEN PER WINST\n" \
    " MIN       ____^\n" \
    " MAX       ____^\n" \
    " GEMIDDELD ____^\n"   // <-- All "____^" get replaced by numbers


// Should be no more than 18 chars
#undef __INTRO_PRESS_START_STR
#define __INTRO_PRESS_START_STR "DRUK OP START"


// X, Y controls upper-left corner coordinates
#undef __INTRO_CREDITS_X
#define __INTRO_CREDITS_X  2u
#undef __INTRO_CREDITS_STR
#define __INTRO_CREDITS_STR \
/*  |--------------------| */\
        "2022\n" \
        "BBBBBR\n" \
        "\n" \
        "THX TO:\n" \
        "ARPRUSS\n" \
        "STACKSMASHING\n" \
        "FERRANTE PESCARA\n" \
        "\n" \
        "GBDK2020\n" \
        "EMULICIOUS"


#endif

