#include <stdint.h>
#ifdef TEST
#include <stdio.h>
#endif

//#pragma bank 3
#include "encoded.h"

uint8_t decodeInt(const uint8_t* blob, uint32_t* valueP) {
    if (blob[0] & 0x80 ) {
        *valueP = blob[0] & 0x7F;
        return 1;
    }
    else if (blob[1] & 0x80) {
        *valueP = blob[0] | ((uint32_t)(blob[1]&0x7F)<<7);
        return 2;
    }
    else /* if (blob[2] & 0x80) */ {
        *valueP = blob[0] | ((uint32_t)(blob[1])<<7) | ((uint32_t)(blob[2]&0x7F)<<14);
        return 3;
    }    
}

void decodeWord(uint8_t start, uint32_t nextFour, char* buffer) {
    buffer[0] = start + 'A';
    buffer[1] = ((uint8_t)(nextFour >> 15) & 0x1F) + 'A';
    buffer[2] = ((uint8_t)(nextFour >> 10) & 0x1F) + 'A';
    buffer[3] = ((uint8_t)(nextFour >> 5) & 0x1F) + 'A';
    buffer[4] = ((uint8_t)(nextFour) & 0x1F) + 'A';
    buffer[5] = 0;
}

void getWord(uint16_t n, char* buffer) {
    uint16_t count = 0;
    uint8_t i;
    for (i = 0 ; i < 26 && n >= words[i+1].wordNumber ; i++) ;
    if (i == 26) {
        *buffer = 0;
        return;
    }
    n -= words[i].wordNumber;
    uint32_t word = 0;
    const uint8_t* blob = wordBlob + words[i].blobOffset;
    uint16_t j;
    for (j=0; j<=n; j++) {
        uint32_t delta;
        blob += decodeInt(blob, &delta);
        word += delta + 1;
    }
    decodeWord(i, word, buffer);
}

uint8_t filterWord(char* s) {
    for (int i=0; i<5; i++)
        if (s[i] < 'A' || s[i] > 'Z')
            return 0;
    uint8_t i = s[0]-'A';
    uint32_t w = ((uint32_t)(s[1]-'A') << 15) | ((uint32_t)(s[2]-'A') << 10) | ((uint32_t)(s[3]-'A') << 5) | (uint32_t)(s[4]-'A');
    uint16_t n = words[i+1].wordNumber - words[i].wordNumber;
    uint32_t match = 0;
    const uint8_t* b = wordBlob + words[i].blobOffset;
    for (uint16_t j=0; j<n; j++) {
        uint32_t delta;
        b += decodeInt(b, &delta);
        match += delta + 1;
        if (match > w)
            return 0;
        else if (match == w)
            return 1;
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
