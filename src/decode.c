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


#define _GETSPECIALWORD_ASM

#ifndef _GETSPECIALWORD_ASM

void getSpecialWord(uint16_t _n, char* buffer) {
    static uint16_t w;
    w = 0;
    const uint8_t* b = answers;
    static uint16_t n;
    n = _n;

    for(;;) {
        static uint8_t c;
        c = *b++;
        if (c) {
            static uint8_t mask;
            for (mask = 1 ; mask ; mask <<= 1) {
                if (c & mask) {
                    if (n == 0) {
                        // Calculate word number "w" using pointer vs base address
                        // b--;
                        w = (uint16_t)(b - answers) << 3;
                        // Then Subtract out unused bits for this byte (rewind mask 1 step) to get final value
                        mask >>= 1;
                        while (mask <<= 1)
                            w--;
                        getWord(w, buffer);
                        return;
                    }
                    n--;
                }
            }
        }
    }
}

#else


// TODO: OLDCALL as precaution against upcoming SDCC calling convention change
void getSpecialWord(uint16_t _n, char* buffer) {

    __asm \

    push AF
    push HL
    push BC
    push DE

    // word number (counter):            bc   :n
    // bit index counter & bitmask loop: de   :w
    // answer array index:               hl   :answer
    // packed-bits:                      a    :c

    // Load _n (arg word number) to bc
    ldhl  sp, #10
    ld    a, (hl+)
    ld    c, a
    ld    a, (hl)
    ld    b, a
    // Counter offsets for word number exit test
    inc b
    inc c

    // Pre-load loop counter (e gets reloaded from d)
    // Load pointer to bit-packed index array
    ld    d, #0x08
    ld    hl, #_answers

    // Load first byte of bit-packed array into a, increment array pointer
    .lookup_loop$:

        // c = *b++;
        ld   a, (hl+)

        // Special handling when value is zero
        // Less cycles when not taken
        // if (c == 0) {
        or    a, a
        jr    z, .lookup_loop$


        // for (mask = 1 ; mask ; mask <<= 1) {
        // e is loop control
        ld     e, d
        .bitmask_loop$:

            // if (c & mask) {
            // Downshift A into carry
            rrca
            // Skip answer word decrement if lowest bit was zero
            jr   nc, .bitmask_loop_check_exit$

            // Otherwise decrement word number counter
            // if (n == 0) return
            dec  c
            jr   nz, .bitmask_loop_check_exit$

            dec  b
            jr   z, .lookup_done$

            .bitmask_loop_check_exit$:
            // for (mask = 1 ; mask ; mask <<= 1) {
            dec    e
        jr     nz, .bitmask_loop$
        // end _bitmask_loop$:

    jr     .lookup_loop$
    // end _lookup_loop$:

    .lookup_done$:
    // Calculate bit index counter address from current pointer and base address
    // w = (uint16_t)(b - answers) << 3;
    ld  a, l
    sub a, #<(_answers)
    ld  l, a

    ld  a, h
    sbc a, #>(_answers)
    ld  h, a

    add hl, hl
    add hl, hl
    add hl, hl

    // Loop value left in e holds unused bits from packed byte,
    // subtract out to get final word index and store in de
    ld    a, l
    sub   a, e
    ld    e, a

    ld    a, h
    sbc   a, #0x00
    ld    d, a

    // Load buffer address onto stack
    ldhl    sp, #12
    ld  a, (hl+)
    ld  c, a
    ld  b, (hl)
    push    bc

    // Load index into bit packed array onto stack
    push    de

    call  _getWord
    add    sp, #4

    pop DE
    pop BC
    pop HL
    pop AF
    ret

    __endasm;

}

#endif

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
