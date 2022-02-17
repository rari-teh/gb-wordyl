#include <stdint.h>
#ifdef TEST
#include <stdio.h>
#endif

#include "encoded.h"

const uint8_t* decodeInt(const uint8_t* blob, uint32_t* valueP) {
    const uint8_t* bb=blob;
    uint8_t b = *bb++;
    uint32_t v = b & 0x7F;
    if (0 == (b & 0x80)) {
        b = *bb++;
        v |= (uint32_t)(b & 0x7F) << 7;
        if (0 == (b&0x80)) {
            v |= (uint32_t)*bb << 14;
            bb++;
        }
    }
    *valueP = v+1;
    return bb;
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
    n -= w->wordNumber;
    uint32_t word = 0;
    const uint8_t* blob = wordBlob + w->blobOffset;
    for (uint16_t j=0; j<=n; j++) {
        uint32_t delta;
        blob = decodeInt(blob, &delta);
        word += delta;
    }
    decodeWord(i, word, buffer);
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
    uint16_t n = words[i+1].wordNumber - words[i].wordNumber;
    uint32_t match = 0;
    const uint8_t* b = wordBlob + words[i].blobOffset;
    for (uint16_t j=0; j<n; j++) {
        uint32_t delta;
        b = decodeInt(b, &delta);
        match += delta;
        if (match >= w) {
            return match == w;
        }
    }
    return 0;
}

void getSpecialWord(int16_t n, char* buffer) {
    uint16_t w = 0;
    const uint8_t* b = answers;
    while(n>=0) {
        uint8_t c = *b;
        if (c == 0) {
            w += 8;
        }
        else {
            for (uint8_t i = 0 ; i < 8 ; i++) {
                if (c & 1) {
                    n--;
                    if (n<0)
                        break;
                }
                w++;
                c >>= 1;
            }
        }
        b++;
    }
    getWord(w, buffer);
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

