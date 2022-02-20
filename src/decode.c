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

#else


// TODO: OLDCALL as precaution against upcoming SDCC calling convention change
void getSpecialWord(uint16_t _n, char* buffer) {

    __asm \

    push AF
    push HL
    push BC
    push DE

    // word number (counter):  bc   :n
    // bit index counter:      de   :w
    // answer array index:     hl   :answer
    // packed-bits:            a    :c

    // Load _n (arg word number) to bc
    ldhl  sp, #10
    ld    a, (hl+)
    ld    c, a
    ld    a, (hl)
    ld    b, a

    // Zero array bit index counter
    // Load pointer to bit-packed index array
    ld    de, #0x0000
    ld    hl, #_answers

    // Load first byte of bit-packed array into a, increment array pointer
    _lookup_loop$:

        // c = *b++;
        ld   a, (hl+)

        // Special handling when value is zero
        // Less cycles when not taken
        // if (c == 0) {
        and   a, #0xFF
        jr    z, _addr_add_8$


        // for (mask = 1 ; mask ; mask <<= 1) {
        // hl will be loop control
        push   hl
        ld     l, #0x08
        _bitmask_loop$:

            // if (c & mask) {
            // Downshift A into carry
            rrca
            // Continue loop if lowest bit was zero
            jr   nc, _bitmask_loop_check_exit$

            // Otherwise decrement word number counter
            // if (n == 0) return
            ld   h, a  // stash a in h
            ld   a, b
            or   a, c
            ld   a, h  // retsore a
            jr   z, _lookup_done$

            // n--;
            dec   bc

            _bitmask_loop_check_exit$:
            // increment bit index counter
            // w++;
            inc   de
            // for (mask = 1 ; mask ; mask <<= 1) {
            dec    l
        jr     nz, _bitmask_loop$
        // end _bitmask_loop$:

        pop    hl
    jr     _lookup_loop$
    // end _lookup_loop$:


        // Special case when bit-packed value is zero
        // w += 8;
        _addr_add_8$:
        ld    a, #0x08
        add   a, e
        ld    e, a
        // handle 8 bit carry for DE
        jr    c, _addr_add_8_c$
        // return to main loop
        jr    _lookup_loop$

        _addr_add_8_c$:
        inc   d

    jr    _lookup_loop$
    // end _lookup_loop$:


    _lookup_done$:
    // this will have been skipped at the end of _lookup_loop
    pop    hl

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
