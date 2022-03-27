// lang_text.h

#ifndef _LANG_TEXT_H
#define _LANG_TEXT_H

// == This has default strings
#include "../lang_text_defaults.inc"

// == #define overrides for language strings go here
// Spanish translation (thank you!)

#undef INTRO_DIALOG_WIN_Y
#define INTRO_DIALOG_WIN_Y DIALOG_TEXT_HEIGHT_LINES(16)
#undef __INTRO_MESSAGE_STR
#define __INTRO_MESSAGE_STR \
/*  |------------------| Available space (18 chars wide) */\
    "ESTO ES WORDYL \n" \
    "\n" \
    "IDIOMA ...... " STR(LANG_CODE) "\n" \
    "  SOLO A..Z\n" \
    "  NO CAR. ESPEC.\n" \
    "\n" \
    "VER MENU:\n" \
    " SELECT x 3\n" \
    "\n" \
    "MOVER CURSOR:\n" \
    "  SELECT + B A\n" \
    "\n" \
    "AUTORELLENAR:\n" \
    "  SELECT + START\n" \
    "\n" \
    "PULSA OTRA TECLA"


#undef __OPTIONS_MENU_STR
#define __OPTIONS_MENU_STR \
/*  |------------------| Available space (18 chars wide) */\
    "OPCIONES\n" \
    "\n" \
    "VER STATS:      A\n" \
    "MODO DIFICIL:   >\n" \
    "AUTORELLENAR:   /\n" \
    "\n" \
    "ABANDONAR:      ^\n" \
    "BORRAR STATS:   <\n" \
    "\n" \
    "SALIR: OTRA TECLA"


#define CONFIRM_DIALOG_WIN_Y  DIALOG_TEXT_HEIGHT_LINES(6)
#undef __CONFIRM_DIALOG_STR
#define __CONFIRM_DIALOG_STR \
/*  |------------------| Available space (18 chars wide) */ \
    "CONFIRMAR:^\n" \
    "CANCELAR: OTRO\n"\
    "..................\n" \
    "\n" \

#undef __CONFIRM_FORFEIT_STR
#define __CONFIRM_FORFEIT_STR     "ABANDONAR?"

#undef __CONFIRM_STATS_RESET_STR
#define __CONFIRM_STATS_RESET_STR "BORRAR STATS?"

#undef __AUTOFILL_ON__STR
#undef __AUTOFILL_OFF__STR
#define __AUTOFILL_ON__STR "AUTORELLENAR: ON"
#define __AUTOFILL_OFF__STR "AUTORELLENAR: OFF"


// Display hard mode indicator at top of screen
#undef OPT_HARDMODE_DISPLAY_X
#undef OPT_HARDMODE_DISPLAY_Y
#define OPT_HARDMODE_DISPLAY_X 5
#define OPT_HARDMODE_DISPLAY_Y 0

#undef __STR_HARD_MODE_DISPLAY
#undef __STR_HARD_MODE_HIDE
#define __STR_HARD_MODE_DISPLAY "DIFICIL"
#define __STR_HARD_MODE_HIDE    "       "


#undef __MESSAGE_HARD_MODE_CANT_CHANGE_STR
#define __MESSAGE_HARD_MODE_CANT_CHANGE_STR \
/*  |------------------| Available space (18 chars wide) */\
    "MODO DIFICIL: SOLO\n" \
    "DISPONIBLE AL\n" \
    "COMENZAR RONDA"


// Avoid increasing this popup height so it doesn't obscure the board
#undef __MESSAGE_HARD_MODE_GUESS_NOT_VALID_STR
#define __MESSAGE_HARD_MODE_GUESS_NOT_VALID_STR \
/*  |------------------| Available space (18 chars wide) */\
    "MODO DIFICIL:\n" \
    "DEBES INCLUIR\n" \
    "TODAS LAS PISTAS"


// Avoid increasing this popup height so it doesn't obscure the board
#undef __MESSAGE_LOSE_STR
#define __MESSAGE_LOSE_STR \
/*  |------------------| Available space (18 chars wide) */\
    "Pierdes!\n" \
    "\n" \
    "Solucion: "    // <-- Needs 5 chars left for answer word!


// NOTE! ^ gets replaced with guess 5 letter word num
//
// Avoid increasing this popup height so it doesn't obscure the board
#undef __MESSAGE_GAME_WON_STR
#define __MESSAGE_GAME_WON_STR \
/*  |------------------| Available space (18 chars wide) */\
    "Ganas!\n" \
    "\n" \
    "Intento ^ of 6"


// Avoid increasing this popup height so it doesn't obscure the board
#undef __MESSAGE_WORD_TOO_SHORT_STR
#define __MESSAGE_WORD_TOO_SHORT_STR \
/*  |------------------| Available space (18 chars wide) */\
    "Demasiado corta!\n" \
    "\n" \
    "5 Letras Min."


// Avoid increasing this popup height so it doesn't obscure the board
#undef __MESSAGE_WORD_NOT_IN_DICT_STR
#define __MESSAGE_WORD_NOT_IN_DICT_STR \
/*  |------------------| Available space (18 chars wide) */\
    "No existe en el\n" \
    "\n" \
    "diccionario!"


#undef __MESSAGE_STATS_RESET_STR
#define __MESSAGE_STATS_RESET_STR \
/*  |------------------| Available space (18 chars wide) */\
    "STATS BORRADAS!"


#undef __STATS_TEMPLATE_STR
#define __STATS_TEMPLATE_STR \
/*  |------------------| Available space (18 chars wide) */\
    " Jugadas ... ____^\n" \
    " Racha ..... ____^\n" \
    " Ganadas ... ____^\n" \
    " Perdidas .. ____^\n" \
    "\n" \
    "Numero de Intentos\n" \
    " Minimo  ... ____^\n" \
    " Maximo  ... ____^\n" \
    " Media...... ____^\n"   // <-- All "____^" get replaced by numbers

// Should be no more than 18 chars
#undef __INTRO_PRESS_START_STR
#define __INTRO_PRESS_START_STR "PULSA START"


// X, Y controls upper-left corner coordinates
#undef __INTRO_CREDITS_STR
#define __INTRO_CREDITS_STR \
/*  |--------------------| */\
        "2022\n" \
        "BBBBBR\n" \
        "\n" \
        "GRACIAS:\n" \
        "ARPRUSS\n" \
        "STACKSMASHING\n" \
        "\n" \
        "GBDK2020\n" \
        "EMULICIOUS"


#endif

