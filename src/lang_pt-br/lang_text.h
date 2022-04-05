// lang_text.h

#ifndef _LANG_TEXT_H
#define _LANG_TEXT_H


// == This has default strings
#include "../lang_text_defaults.inc"


// == #define overrides for language strings go here

#undef __INTRO_MESSAGE_STR
#define __INTRO_MESSAGE_STR \
/*  |------------------| Available space (18 chars wide) */\
    "WELCOME TO WORDYL \n" \
    "\n" \
    "LANGUAGE ... " STR(LANG_CODE) "\n" \
    "  A..Z  ONLY\n" \
    "  NO SPECIAL CHARS\n" \
    "\n" \
    "IN GAME MENU:\n" \
    "  3 x SELECT\n" \
    "\n" \
    "BOARD CURSOR:\n" \
    "  SELECT + B A\n" \
    "\n" \
    "AUTO FILL:\n" \
    "  SELECT + START\n" \
    "\n" \
    "ANY KEY TO BEGIN"


#endif