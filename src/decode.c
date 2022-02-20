#include <stdint.h>
#ifdef TEST
#include <stdio.h>
#endif

#include "encoded.h"

static uint32_t currentWord;
static const uint8_t* blobPtr;

void updateWord(void) {
    uint8_t b = *blobPtr++;
    uint32_t v;
    v = b & 0x7F;
    if (0 == (b & 0x80)) {
        b = *blobPtr++;
        v |= (uint32_t)(b & 0x7F) << 7;
        if (0 == (b&0x80)) {
            v |= (uint32_t)*blobPtr << 14;
            blobPtr++;
        }
    }
    currentWord += v+1;
}

void decodeWord(uint8_t start, uint32_t nextFour, char* buffer) {
    *buffer = start + 'A';
    buffer += 5;
    *buffer-- = 0;
    for(uint8_t i=1;i<5;i++) {
        *buffer-- = (nextFour & 0x1F) + 'A';
        nextFour >>= 5;
    }
}

void getWord(uint16_t n, char* buffer) {
    uint16_t count = 0;
    uint8_t i;
    const LetterList_t* w;
    w = words;
    for (i = 0 ; i < 26 && n >= w[1].wordNumber ; i++, w++) ;
    if (i == 26) {
        *buffer = 0;
        return;
    }
    currentWord = 0;
    blobPtr = wordBlob + w->blobOffset;
    for (uint16_t j = n - w->wordNumber + 1; j; j--) {
        updateWord();
    }
    decodeWord(i, currentWord, buffer);
}

uint8_t filterWord(char* s) {
    uint8_t i;
    /* because the input system uses an on-screen keyboard with A-Z only, no need to sanitize */
    /* for (i=0; i<5; i++)
        if (s[i] < 'A' || s[i] > 'Z')
            return 0; */
    uint32_t w = 0;
    for (i=1;i<5;i++)
        w = (w << 5) | (s[i]-'A');
    
    i = s[0]-'A';
    currentWord = 0;
    blobPtr = wordBlob + words[i].blobOffset;
    for (uint16_t j=words[i+1].wordNumber - words[i].wordNumber; j; j--) {
        updateWord();
        if (currentWord >= w) {
            return currentWord == w;
        }
    }
    return 0;
}

void getSpecialWord(uint16_t _n, char* buffer) {
    static uint16_t w;
    w = 0;
    const uint8_t* b = answers;
    static uint16_t n;
    n = _n;

    for(;;) { 
        static uint8_t c;
        c = *b++;
        if (c == 0) {
            w += 8;
        }
        else {
            static uint8_t mask;
            for (mask = 1 ; mask ; mask <<= 1) {
                if (c & mask) {
                    if (n == 0) {
                        getWord(w, buffer);
                        return;
                    }
                    n--;
                }
                w++;
            }
        }
    }
}

#ifdef TEST
main() {
    char w[6] = {0};
    for (int i=0; i<100; i++) {
        getSpecialWord(i, w);
        puts(w);
    }
    printf("%d\n", filterWord("SEREZ"));
    printf("%d\n", filterWord("SERER"));
    printf("%d\n", filterWord("ZYMIC"));
    printf("%d\n", filterWord("BAAED"));
}
#endif
