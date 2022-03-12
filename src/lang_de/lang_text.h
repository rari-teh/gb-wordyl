// lang_text.h

#ifndef _LANG_TEXT_H
#define _LANG_TEXT_H

// == This has default strings
#include "../lang_text_defaults.inc"


// == #define overrides for language strings go here

// German

#undef __INTRO_MESSAGE_STR
#define __INTRO_MESSAGE_STR \
/*  |------------------| Available space (18 chars wide) */\
    "WILLKOMMEN BEI\n" \
    "WORDYL\n" \
    "\n" \
    "SPRACHE ....... " STR(LANG_CODE) "\n" \
    "  NUR A..Z\n" \
    "  KEINE SONDERZCHN\n" \
    "\n" \
    "IN GAME MENU:  SEL\n" \
    "\n" \
    "TASTE ZUM STARTEN"


#undef __OPTIONS_MENU_STR
#define __OPTIONS_MENU_STR \
/*  |------------------| Available space (18 chars wide) */\
    "OPTIONEN\n\n" \
    "ZEIEG STATS:     B\n" \
    "\n" \
    "NULL STATS:     RT\n" \
    "RUNDE AUFGEBEN: UP\n" \
    "\n" \
    "TASTE ZUM WEITER"


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
    "AUF RATE X von 6"


#undef __MESSAGE_WORD_TOO_SHORT_STR
#define __MESSAGE_WORD_TOO_SHORT_STR \
/*  |------------------| Available space (18 chars wide) */\
    "WORT IST ZU KURZ!\n" \
    "\n" \
    "BRAUCHT 5 ZEICHEN"

#undef __MESSAGE_WORD_NOT_IN_DICT_STR
#define __MESSAGE_WORD_NOT_IN_DICT_STR \
/*  |------------------| Available space (18 chars wide) */\
    "WORT IST NICHT\n" \
    "\n" \
    "IN DER DATENBANK!"


#undef __MESSAGE_STATS_RESET_STR
#define __MESSAGE_STATS_RESET_STR \
/*  |------------------| Available space (18 chars wide) */\
    "STATS NEUSTARTEN!"


#undef __STATS_TEMPLATE_STR
#define __STATS_TEMPLATE_STR \
/*  |------------------| Available space (18 chars wide) */\
    " Runden   ____X\n" \
    " Streak   ____X\n" \
    " Gewinnt  ____X\n" \
    " Verluste ____X\n" \
    "\n" \
    "Vermutungen zu\n" \
    "gewinnen\n" \
    " Least    ____X\n" \
    " Most     ____X\n" \
    " Average  ____X\n"   // <-- All "____X" get replaced by numbers


// Should be no more than 12 chars
#undef __INTRO_PRESS_START_STR
#define __INTRO_PRESS_START_STR "PUSHEN START"

#endif