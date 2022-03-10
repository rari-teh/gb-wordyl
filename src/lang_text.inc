#include <gbdk/platform.h>
#include <stdint.h>
#include <stdbool.h>

#include "../common.h"

uint8_t intro_message_str[] =
// 1       9 1      8
  "WELCOME TO WORDYL \n"
  "\n"
  "LANGUAGE ...... " STR(LANG_CODE) "\n"
// #if (EXPAND(LANG_CODE) != en)
#ifndef LANG_IS_EN
  "  A..Z  ONLY\n"
  "  NO SPECIAL CHARS\n"
  "\n"
#endif
  "IN GAME MENU . SEL\n"
  "\n"
  "ANY KEY TO START";
