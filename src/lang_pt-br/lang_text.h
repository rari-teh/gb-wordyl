// lang_text.h

#ifndef _LANG_TEXT_H
#define _LANG_TEXT_H


// == This has default strings
#include "../lang_text_defaults.inc"


// == #define overrides for language strings go here

#undef INTRO_DIALOG_WIN_Y
#define INTRO_DIALOG_WIN_Y DIALOG_TEXT_HEIGHT_LINES(16)
#undef __INTRO_MESSAGE_STR
#define __INTRO_MESSAGE_STR \
/*  |------------------| Available space (18 chars wide) */\
    "BEMVINDO AO WORDYL\n" \
    "\n" \
    "IDIOMA ... " STR(LANG_CODE) "\n" \
    "  A..Z  APENAS\n" \
    "  SEM CAR. ESPEC.\n" \
    "\n" \
    "ABRIR MENU:\n" \
    "  3 x SELECT\n" \
    "\n" \
    "SELECIONAR:\n" \
    "  SELECT + BA\n" \
    "\n" \
    "PREENCHER:\n" \
    "  SELECT + START\n" \
    "\n" \
    "APERTE PRA COMECAR"

#undef __OPTIONS_MENU_STR
#define __OPTIONS_MENU_STR \
/*  |------------------| Available space (18 chars wide) */\
    "OPCOES\n" \
    "\n" \
    "VER STATS:     A\n" \
    "MODO DIFICIL:  >\n" \
    "AUTOPREENCHER: /\n" \
    "\n" \
    "SAIR:          ^\n" \
    "APAGAR STATS:  <\n" \
    "\n" \
    "SAIR: APERTE"


#define CONFIRM_DIALOG_WIN_Y  DIALOG_TEXT_HEIGHT_LINES(6)
#undef __CONFIRM_DIALOG_STR
#define __CONFIRM_DIALOG_STR \
/*  |------------------| Available space (18 chars wide) */ \
    "CONFIRMAR:^\n" \
    "CANCELAR: OUTRA\n"\
    "..................\n" \
    "\n" \

#undef __CONFIRM_FORFEIT_STR
#define __CONFIRM_FORFEIT_STR     "SAIR?"

#undef __CONFIRM_STATS_RESET_STR
#define __CONFIRM_STATS_RESET_STR "APAGAR STATS?"

#undef __AUTOFILL_ON__STR
#undef __AUTOFILL_OFF__STR
#define __AUTOFILL_ON__STR "AUTOPREENCHER: ON"
#define __AUTOFILL_OFF__STR "AUTOPREENCHER: OFF"


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
    "MUDA APENAS EM\n" \
    "NOVA RODADA"


// Avoid increasing this popup height so it doesn't obscure the board
#undef __MESSAGE_HARD_MODE_GUESS_NOT_VALID_STR
#define __MESSAGE_HARD_MODE_GUESS_NOT_VALID_STR \
/*  |------------------| Available space (18 chars wide) */\
    "MODO DIFICIL:\n" \
    "DEVE INCLUIR\n" \
    "DICAS REVELADAS"


// Avoid increasing this popup height so it doesn't obscure the board
#undef __MESSAGE_LOSE_STR
#define __MESSAGE_LOSE_STR \
/*  |------------------| Available space (18 chars wide) */\
    "Perdeu!\n" \
    "\n" \
    "Resposta: "    // <-- Needs 5 chars left for answer word!


// NOTE! ^ gets replaced with guess 5 letter word num
//
// Avoid increasing this popup height so it doesn't obscure the board
#undef __MESSAGE_GAME_WON_STR
#define __MESSAGE_GAME_WON_STR \
/*  |------------------| Available space (18 chars wide) */\
    "Ganhou!\n" \
    "\n" \
    "Tentativa ^ DE 6"


// Avoid increasing this popup height so it doesn't obscure the board
#undef __MESSAGE_WORD_TOO_SHORT_STR
#define __MESSAGE_WORD_TOO_SHORT_STR \
/*  |------------------| Available space (18 chars wide) */\
    "Termo muito curto!\n" \
    "\n" \
    "Min. de 5 Letras"


// Avoid increasing this popup height so it doesn't obscure the board
#undef __MESSAGE_WORD_NOT_IN_DICT_STR
#define __MESSAGE_WORD_NOT_IN_DICT_STR \
/*  |------------------| Available space (18 chars wide) */\
    "Palava nao esta no\n" \
    "\n" \
    "dicionario!"


#undef __MESSAGE_STATS_RESET_STR
#define __MESSAGE_STATS_RESET_STR \
/*  |------------------| Available space (18 chars wide) */\
    "STATS APAGADOS!"


#undef __STATS_TEMPLATE_STR
#define __STATS_TEMPLATE_STR \
/*  |------------------| Available space (18 chars wide) */\
    " Jogadas ... ____^\n" \
    " Seguida ... ____^\n" \
    " Vitoria ... ____^\n" \
    " Perdidas .. ____^\n" \
    "\n" \
    "Numero de Jogadas\n" \
    " Minimo  ... ____^\n" \
    " Maximo  ... ____^\n" \
    " Media...... ____^\n"   // <-- All "____^" get replaced by numbers

// Should be no more than 18 chars
#undef __INTRO_PRESS_START_STR
#define __INTRO_PRESS_START_STR "APERTE START"


// X, Y controls upper-left corner coordinates
#undef __INTRO_CREDITS_STR
#define __INTRO_CREDITS_STR \
/*  |--------------------| */\
        "Obrigado a:\n" \
        "TOXA\n" \
        "ARPRUSS\n" \
        "STACKSMASHING\n" \
        "\n" \
        "GBDK2020\n" \
        "EMULICIOUS\n" \
        "FERRANTE CRAFTS"


#endif