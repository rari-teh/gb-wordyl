// decode.h

#ifndef _DECODE_H
#define _DECODE_H


uint8_t filterWord(char* s);
void getWord(uint16_t dictWordNum);
void getSpecialWord(uint16_t n, char* buffer) OLDCALL;

void buildRAMBuckets(void);
void dumpAnswersToEmuConsole(void);
#include <sizes.h>

#endif
