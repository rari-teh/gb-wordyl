#include <gbdk/platform.h>
#include <stdint.h>
#include <stdbool.h>

#include "../common.h"
#include "../window.h"

#ifndef LANG_IS_EN

    #define INTRO_DIALOG_WIN_Y DIALOG_TEXT_HEIGHT_LINES(9)
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

    #define INTRO_DIALOG_WIN_Y DIALOG_TEXT_HEIGHT_LINES(7)
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


#define OPTIONS_MENU_DIALOG_WIN_Y  DIALOG_TEXT_HEIGHT_LINES(9)
#define __OPTIONS_MENU_STR \
/*  |------------------| Available space (18 chars wide) */\
    "OPTIONS\n" \
    "\n" \
    "SHOW STATS ....  B\n" \
    "HARD MODE .....  A\n" \
    "\n" \
    "RESET STATS ...  >\n" \
    "FORFEIT ROUND .  ^\n" \
    "\n" \
    "EXIT MENU .... ANY"


// Avoid increasing this popup height so it doesn't obscure the board
#define LOSE_MESSAGE_DIALOG_WIN_Y  DIALOG_TEXT_HEIGHT_LINES(3)
#define __MESSAGE_LOSE_STR \
/*  |------------------| Available space (18 chars wide) */\
    "You lose. Sorry!\n" \
    "\n" \
    "Answer is: "    // <-- Needs 5 chars left for answer word!


// Display hard mdoe indicator just above keyboard on left
#define OPT_HARDMODE_DISPLAY_X 15
#define OPT_HARDMODE_DISPLAY_Y 12
#define __STR_HARD_MODE_DISPLAY "HARD"


// Avoid increasing this popup height so it doesn't obscure the board
#define HARD_MODE_GUESS_NOT_VALID_WIN_Y  DIALOG_TEXT_HEIGHT_LINES(3)
#define __MESSAGE_HARD_MODE_GUESS_NOT_VALID_STR \
/*  |------------------| Available space (18 chars wide) */\
    "HARD MODE: GUESS\n" \
    "MUST HAVE ALL\n" \
    "REVEALED CLUES"


// NOTE! X gets replaced with guess 5 letter word num
//
// Avoid increasing this popup height so it doesn't obscure the board
#define WON_MESSAGE_DIALOG_WIN_Y  DIALOG_TEXT_HEIGHT_LINES(3)
#define __MESSAGE_GAME_WON_STR \
/*  |------------------| Available space (18 chars wide) */\
    "You won!\n" \
    "\n" \
    "On Guess ^ of 6"


// Avoid increasing this popup height so it doesn't obscure the board
#define WORD_TOO_SHORT_DIALOG_WIN_Y  DIALOG_TEXT_HEIGHT_LINES(3)
#define __MESSAGE_WORD_TOO_SHORT_STR \
/*  |------------------| Available space (18 chars wide) */\
    "Word is too short!\n" \
    "\n" \
    "Needs 5 Letters"


// Avoid increasing this popup height so it doesn't obscure the board
#define WORD_NOT_IN_DICT_DIALOG_WIN_Y  DIALOG_TEXT_HEIGHT_LINES(3)
#define __MESSAGE_WORD_NOT_IN_DICT_STR \
/*  |------------------| Available space (18 chars wide) */\
    "Word is not in\n" \
    "\n" \
    "dictionary!"


#define STATS_RESET_DIALOG_WIN_Y  DIALOG_TEXT_HEIGHT_LINES(3)
#define __MESSAGE_STATS_RESET_STR \
/*  |------------------| Available space (18 chars wide) */\
    "STATS RESET!"


#define STATS_DIALOG_WIN_Y  DIALOG_TEXT_HEIGHT_LINES(9)
#define __STATS_TEMPLATE_STR \
/*  |------------------| Available space (18 chars wide) */\
    " Played  ____^\n" \
    " Streak  ____^\n" \
    " Wins    ____^\n" \
    " Losses  ____^\n" \
    "\n" \
    "Guesses to Win\n" \
    " Least   ____^\n" \
    " Most    ____^\n" \
    " Average ____^\n"   // <-- All "____X" get replaced by numbers

// Should be no more than 12 chars
#define __INTRO_PRESS_START_STR "PRESS START"


// X, Y controls upper-left corner coordinates
#define __INTRO_CREDITS_X  4u
#define __INTRO_CREDITS_Y  (DEVICE_SCREEN_HEIGHT - 9u) / 2u
#define __INTRO_CREDITS_STR \
/*  |--------------------| */\
        "2022\n" \
        "BBBBBR\n" \
        "\n" \
        "THX TO:\n" \
        "ARPRUSS\n" \
        "STACKSMASHING\n" \
        "\n" \
        "GBDK2020\n" \
        "EMULICIOUS"
