// lang_text.h

#ifndef _LANG_TEXT_H
#define _LANG_TEXT_H

// == This has default strings
#include "../lang_text_defaults.inc"


// == #define overrides for language strings go here

// German (edit+fix by skite2001 ^_-)

#undef INTRO_DIALOG_WIN_Y
#define INTRO_DIALOG_WIN_Y DIALOG_TEXT_HEIGHT_LINES(10)
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
    "OPTIONEN:   SELECT\n" \
    "\n" \
    "TASTE DRUECKEN"


#undef __OPTIONS_MENU_STR
#define __OPTIONS_MENU_STR \
/*  |------------------| Available space (18 chars wide) */\
    "OPTIONEN\n" \
    "\n" \
    "ZEIGE STATS:     B\n" \
    "\n" \
    "STATS ERNEUERN: RT\n" \
    "RUNDE AUFGEBEN: UP\n" \
    "\n" \
    "TASTE DRUECKEN"


#undef __MESSAGE_LOSE_STR
#define __MESSAGE_LOSE_STR \
/*  |------------------| Available space (18 chars wide) */\
    "DU VERLIERST!\n" \
    "\n" \
    "ANTWORT IST: "    // <-- Needs 5 chars left for answer word

// NOTE! X gets replaced with guess 5 letter word num
//
// Intentionally not const! X gets replaced with guess num
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