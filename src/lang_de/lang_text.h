// lang_text.h

#ifndef _LANG_TEXT_H
#define _LANG_TEXT_H

// == This has default strings
#include "../lang_text_defaults.inc"


// == #define overrides for language strings go here

// German (edit+fix by skite2001 ^_-)

#undef INTRO_DIALOG_WIN_Y
#define INTRO_DIALOG_WIN_Y DIALOG_TEXT_HEIGHT_LINES(16)
#undef __INTRO_MESSAGE_STR
#define __INTRO_MESSAGE_STR \
/*  |------------------| Available space (18 chars wide) */\
    "WILLKOMMEN BEI\n" \
    "WORDYL\n" \
    "\n" \
    "SPRACHE ....... " STR(LANG_CODE) "\n" \
    "  NUR A..Z OHNE\n" \
    "  SONDERZEICHEN\n" \
    "\n" \
    "OPTIONEN:3x SELECT\n" \
    "\n" \
    "STELLENAUSWAHL:\n" \
    "  SELECT + B A\n" \
    "\n" \
    "AUTOEINGABE:\n" \
    "  SELECT + START\n" \
    "\n" \
    "TASTE DRUECKEN"


#undef __OPTIONS_MENU_STR
#define __OPTIONS_MENU_STR \
/*  |------------------| Available space (18 chars wide) */\
    "OPTIONEN\n" \
    "\n" \
    "ZEIGE STATS:     A\n" \
    "PROFI MODUS:     >\n" \
    "AUTOEINGABE:     /\n" \
    "\n" \
    "RUNDE AUFGEBEN:  ^\n" \
    "STATS ERNEUERN:  <\n" \
    "\n" \
    "TASTE DRUECKEN"


#define CONFIRM_DIALOG_WIN_Y  DIALOG_TEXT_HEIGHT_LINES(6)
#undef __CONFIRM_DIALOG_STR
#define __CONFIRM_DIALOG_STR \
/*  |------------------| Available space (18 chars wide) */ \
    "WEITER:^\n" \
    "STOP: ANDERE TASTE\n"\
    "..................\n" \
    "\n" \

#undef __CONFIRM_FORFEIT_STR
#define __CONFIRM_FORFEIT_STR     "RUNDE AUFGEBEN?"

#undef __CONFIRM_STATS_RESET_STR
#define __CONFIRM_STATS_RESET_STR "STATS ERNEUERN?"

#undef __AUTOFILL_ON__STR
#undef __AUTOFILL_OFF__STR
#define __AUTOFILL_ON__STR "AUTOEINGABE: AN"
#define __AUTOFILL_OFF__STR "AUTOEINGABE: AUS"


// Display hard mode indicator just above keyboard on right
#undef OPT_HARDMODE_DISPLAY_X
#define OPT_HARDMODE_DISPLAY_X 14
#undef __STR_HARD_MODE_DISPLAY
#undef __STR_HARD_MODE_HIDE
#define __STR_HARD_MODE_DISPLAY "PROFI" // 5 chars max at this location
#define __STR_HARD_MODE_HIDE    "     "


#undef __MESSAGE_HARD_MODE_CANT_CHANGE_STR
#define __MESSAGE_HARD_MODE_CANT_CHANGE_STR \
/*  |------------------| Available space (18 chars wide) */\
    "PROFI MODUS: NUR\n" \
    "BEI NEUER RUNDE\n" \
    "WECHSELBAR"


#undef __MESSAGE_HARD_MODE_GUESS_NOT_VALID_STR
#define __MESSAGE_HARD_MODE_GUESS_NOT_VALID_STR \
/*  |------------------| Available space (18 chars wide) */\
    "PROFI MODUS:\n" \
    "RICHTIGE HINWEISE\n" \
    "SIND BINDEND!"



#undef __MESSAGE_LOSE_STR
#define __MESSAGE_LOSE_STR \
/*  |------------------| Available space (18 chars wide) */\
    "DU VERLIERST!\n" \
    "\n" \
    "ANTWORT IST: "    // <-- Needs 5 chars left for answer word


// NOTE! ^ gets replaced with guess 5 letter word num
//
// Intentionally not const! ^ gets replaced with guess num
#undef __MESSAGE_GAME_WON_STR
#define __MESSAGE_GAME_WON_STR \
/*  |------------------| Available space (18 chars wide) */\
    "DU HAST GEWONNEN!\n" \
    "\n" \
    "VERSUCH ^ von 6"


#undef __MESSAGE_WORD_TOO_SHORT_STR
#define __MESSAGE_WORD_TOO_SHORT_STR \
/*  |------------------| Available space (18 chars wide) */\
    "WORT IST ZU KURZ!\n" \
    "\n" \
    "5 BUCHSTABEN BITTE"

#undef __MESSAGE_WORD_NOT_IN_DICT_STR
#define __MESSAGE_WORD_NOT_IN_DICT_STR \
/*  |------------------| Available space (18 chars wide) */\
    "WORT IST NICHT\n" \
    "\n" \
    "IN DER DATENBANK!"


#undef __MESSAGE_STATS_RESET_STR
#define __MESSAGE_STATS_RESET_STR \
/*  |------------------| Available space (18 chars wide) */\
    "STATS ERNEUERT!"


#undef __STATS_TEMPLATE_STR
#define __STATS_TEMPLATE_STR \
/*  |------------------| Available space (18 chars wide) */\
    " RUNDEN   ____^\n" \
    " SERIE    ____^\n" \
    " GEWONNEN ____^\n" \
    " VERLOREN ____^\n" \
    "\n" \
    "GEWINNVERSUCHE\n" \
    " MIN      ____^\n" \
    " MAX      ____^\n" \
    " MITTE    ____^\n"   // <-- All "____^" get replaced by numbers


// Should be no more than 18 chars
#undef __INTRO_PRESS_START_STR
#define __INTRO_PRESS_START_STR "DRUECKE START"


// X, Y controls upper-left corner coordinates
#undef __INTRO_CREDITS_STR
#define __INTRO_CREDITS_STR \
/*  |--------------------| */\
        "2022\n" \
        "BBBBBR\n" \
        "\n" \
        "THX TO:\n" \
        "ARPRUSS\n" \
        "STACKSMASHING\n" \
        "SKITE2001\n" \
        "\n" \
        "GBDK2020\n" \
        "EMULICIOUS"


#endif
