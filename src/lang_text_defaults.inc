#include <gbdk/platform.h>
#include <stdint.h>
#include <stdbool.h>

#include "../common.h"

#ifndef LANG_IS_EN

    #define __INTRO_MESSAGE_STR \
    /*  |------------------| Available space (18 chars wide) */\
        "WELCOME TO WORDYL \n" \
        "\n" \
        "LANGUAGE ...... " STR(LANG_CODE) "\n" \
        "  A..Z  ONLY\n" \
        "  NO SPECIAL CHARS\n" \
        "\n" \
        "IN GAME MENU . SEL\n" \
        "\n" \
        "ANY KEY TO START"

#else

    #define __INTRO_MESSAGE_STR \
    /*  |------------------| Available space (18 chars wide) */\
        "WELCOME TO WORDYL \n" \
        "\n" \
        "LANGUAGE ...... " STR(LANG_CODE) "\n" \
        "\n" \
        "IN GAME MENU . SEL\n" \
        "\n" \
        "ANY KEY TO START"

#endif


#define __OPTIONS_MENU_STR \
/*  |------------------| Available space (18 chars wide) */\
    "OPTIONS\n\n" \
    "SHOW STATS ....  B\n\n" \
    "RESET STATS ... RT\n" \
    "FORFEIT ROUND . UP\n\n" \
    "EXIT MENU .... ANY"


#define __MESSAGE_LOSE_STR \
/*  |------------------| Available space (18 chars wide) */\
    "You lose. Sorry!\n" \
    "\n" \
    "Answer is: "    // <-- Needs 5 chars left for answer word!


// NOTE! X gets replaced with guess 5 letter word num
//
// Intentionally not const! X gets replaced with guess num
#define __MESSAGE_GAME_WON_STR \
/*  |------------------| Available space (18 chars wide) */\
    "You won!\n" \
    "\n" \
    "On Guess X of 6"


#define __MESSAGE_WORD_TOO_SHORT_STR \
/*  |------------------| Available space (18 chars wide) */\
    "Word is too short!\n" \
    "\n" \
    "Needs 5 Letters"

#define __MESSAGE_WORD_NOT_IN_DICT_STR \
/*  |------------------| Available space (18 chars wide) */\
    "Word is not in\n" \
    "\n" \
    "dictionary!"

#define __MESSAGE_STATS_RESET_STR \
/*  |------------------| Available space (18 chars wide) */\
    "STATS RESET!"

#define __STATS_TEMPLATE_STR \
/*  |------------------| Available space (18 chars wide) */\
    " Played  ____X\n" \
    " Streak  ____X\n" \
    " Wins    ____X\n" \
    " Losses  ____X\n" \
    "\n" \
    "Guesses to Win\n" \
    " Least   ____X\n" \
    " Most    ____X\n" \
    " Average ____X\n"   // <-- All "____X" get replaced by numbers

// Should be no more than 12 chars
#define __INTRO_PRESS_START_STR "PRESS START"

