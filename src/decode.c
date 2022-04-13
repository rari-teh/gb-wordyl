#include <stdint.h>
#include <stdbool.h>
#include <stdbool.h>

#include <asm/types.h>
#include "encoded.h"


#define _ASM_GETSPECIALWORD
#define _ASM_UPDATEWORD

static uint32_t currentWord;
static const uint8_t* blobPtr;
static char     * str_return_buffer;


#ifdef _ASM_UPDATEWORD

// high byte of currentWord is always zero
void updateWord(void) OLDCALL {
__asm
    ld a,(#_blobPtr)
    ld l,a
    ld a,(#_blobPtr+1)
    ld h,a
    ld a,(hl+)
    bit 7,a
    jr z,0001$
    and #127
    ld b,a
    xor a
    ld d,a
0005$:
    ld e,a
0003$:
;; inc edb
    ld a,b
    add #1
    ld b,a
    ld a,d
    adc #0
    ld d,a
    ld a,e
    adc #0
    ld e,a
;; add edb to currentWord
    ld a,(#_currentWord)
    add b
    ld (#_currentWord),a
    ld a,(#_currentWord+1)
    adc d
    ld (#_currentWord+1),a
    ld a,(#_currentWord+2)
    adc e
    ld (#_currentWord+2),a
;; update blobPtr
    ld a,l
    ld (#_blobPtr),a
    ld a,h
    ld (#_blobPtr+1),a
    ret
0001$:
    ld b,a
    ld a,(hl+)
    ld c,a
    and #127
    ld d,a
    srl d
    sla a
    sla a
    sla a
    sla a
    sla a
    sla a
    sla a
    or b
    ld b,a
    bit 7,c
    jr z,0002$
    xor a
    jr 0005$
0002$:
    ld a,(hl+)
    ld e,a
    srl e
    srl e
    sla a
    sla a
    sla a
    sla a
    sla a
    sla a
    or d
    ld d,a
    jr 0003$
__endasm ;
}

#else

void updateWord(void) OLDCALL {
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

#endif // _ASM_UPDATEWORD

#define ALPHABET_REMAP
#define WORD_LETTERS_REVERSED

#ifdef ALPHABET_REMAP
    const char  alpha_unmap[] = "AEIOUSTRYHKBCDFGJLMNPQVWXZ";
    //                             [ ^un-map dir ]
    //                           ABCDEFGHIJKLMNOPQRSTUVWXYZ
    //                             [ map dir \/ ]
    const char alpha_newmap[] = "ALMNBOPJCQKRSTDUVHFGEWXYIZ";

    // Optional full-word alphabet remapping

    // // Applies alphabet remapping to a string
    // void alpha_apply_remap(char * str_word) {
    //     for (uint8_t c = 0; c < 5; c++)
    //         str_word[c] = alpha_newmap[str_word[c] - 'A'];
    // }

    // // Reverse alphabet remapping that was previously applied to a string
    // void alpha_remove_remap(char * str_word) {
    //     for (uint8_t c = 0; c < 5; c++)
    //         str_word[c] = alpha_unmap[str_word[c] - 'A'];
    // }

#endif


// Applies alphabet remapping to a character
inline char check_alpha_remap_char(char c) {
    #ifdef ALPHABET_REMAP
        return alpha_newmap[c - 'A'];
    #else
        return c;
    #endif
}

// Reverse alphabet remapping that was previously applied to a character
inline char check_alpha_unmap_char(char c) {
    #ifdef ALPHABET_REMAP
        return alpha_unmap[c - 'A'];
    #else
        return c;
    #endif
}


// Converts a word from a dictionary bucket # and numeric value to a string
void decodeWord(uint8_t start, uint32_t nextFour, char* buffer) {

    #ifdef WORD_LETTERS_REVERSED
        // Words encoded with letters in reverse order
        *(buffer+5) = 0;
        // Set last letter from letter bucket index (a-z)
        *(buffer+4) = check_alpha_unmap_char(start + 'A');
        // Decode remaining 4 letters
        for(uint8_t i=1;i<5;i++) {
            *buffer++ = check_alpha_unmap_char((nextFour & 0x1F) + 'A');
            nextFour >>= 5;
        }
    #else
        // Words encoded with normal letter order
        *buffer = check_alpha_unmap_char(start + 'A');
        buffer += 5;
        *buffer-- = 0;
        for(uint8_t i=1;i<5;i++) {
            *buffer-- = check_alpha_unmap_char((nextFour & 0x1F) + 'A');
            nextFour >>= 5;
        }
    #endif
}


// Gets an answer word (from the dictionary) by index number in
// the answer list
void getWord(uint16_t n) {
    uint16_t count = 0;
    uint8_t i;
    const LetterBucket_t* w;
    w = buckets;
    for (i = 0 ; i < 26 && n >= w[1].wordNumber ; i++, w++) ;
    if (i == 26) {
        *str_return_buffer = 0;
        return;
    }
    currentWord = 0;
    blobPtr = wordBlob + w->blobOffset;
    for (uint16_t j = n - w->wordNumber + 1; j; j--) {
        updateWord();
    }
    decodeWord(i, currentWord, str_return_buffer);
}

// Check to see if the guess word is in the dictionary.
//
// Encodes guess word to 5-bits per letter, then seeks through
// dictionary looking for the numeric match
uint8_t filterWord(char* s) {
    /* because the input system uses an on-screen keyboard with A-Z only, no need to sanitize */
    /* for (i=0; i<5; i++)
        if (s[i] < 'A' || s[i] > 'Z')
            return 0; */
    uint32_t w = 0;

    #ifdef WORD_LETTERS_REVERSED
        int8_t i;
        // Words encoded with letters in reverse order
        for (i=3;i>=0;i--)
            w = (w << 5) | (check_alpha_remap_char(s[i])-'A');

        i = check_alpha_remap_char(s[4])-'A';
    #else
        uint8_t i;
        // Words encoded with normal letter order
        for (i=1;i<5;i++)
            w = (w << 5) | (check_alpha_remap_char(s[i])-'A');

        i = check_alpha_remap_char(s[0])-'A';
    #endif

    currentWord = 0;
    blobPtr = wordBlob + buckets[i].blobOffset;
    for (uint16_t j=buckets[i+1].wordNumber - buckets[i].wordNumber; j; j--) {
        updateWord();
        if (currentWord >= w) {
            return currentWord == w;
        }
    }

    return 0;
}


#ifndef _ASM_GETSPECIALWORD

// C version is about 70-80 bytes larger than ASM version right now,
// speed comparable once bucket size is increased to 50
void getSpecialWord(uint16_t _n, char* buffer) OLDCALL {

    str_return_buffer = buffer;
    static uint16_t w;
    w = 0;
    static uint16_t n;
    n = _n;

    const AnswerBucket_t* bucket = answerBuckets;
    const uint8_t* b = answers;

    // Use bucket index to fast-forward closer to desired answer word
    while (bucket->numWords <= n) {
        n -= bucket->numWords;
        b += bucket->byteOffsetDelta;
        bucket++;
    }

    while(1) {
        static uint8_t c;
        c = *b++;
        if (c) {
            static uint8_t mask;
            for (mask = 1 ; mask ; mask <<= 1) {
                if (c & mask) {
                    if (n == 0) {
                        // Calculate word number "w" using pointer vs base address
                        w = (uint16_t)(b - answers) << 3;
                        // Then Subtract out unused bits for this byte (rewind mask 1 step) to get final value
                        mask >>= 1;
                        while (mask <<= 1)
                            w--;
                        getWord(w);
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
void getSpecialWord(uint16_t special_word_num, char* str_buffer) OLDCALL {

    __asm \

    push AF
    push HL
    push BC
    push DE

    // word number (counter):            bc   :n
    // bit index counter & bitmask loop: de   :w
    // answer array index:               hl   :answer
    // packed-bits:                      a    :c

    // Load str_buffer into global
    ldhl  sp, #12
    ld    a, (hl+)
    ld    (#_str_return_buffer), a
    ld    a, (hl)
    ld    (#_str_return_buffer + 1), a

    // Load _n (arg word number) to bc
    ldhl  sp, #10
    ld    a, (hl+)
    ld    c, a
    ld    a, (hl)
    ld    b, a


    // const AnswerBucket_t* bucket = answerBuckets;
    ld    de, #_answers
    ld    hl, #_answerBuckets
    .preload_loop$:

        push  de
        // while (answer_word_num >= bucket->numWords)
        // bc: answer_word_num, hl: answerBuckets->numWords
        // save result in e to avoid re-subtracting if applicable
        ld    a, c
        sub   a, (hl)
        ld    e, a
        ld    a, b
        sbc   a, #0x00
        // If answer_word_num was smaller, exit loop
        jr    c, .preload_loop_done$
        // True: (answer_word_num >= bucket->numWords)
        // save result from above: answer_word_num -= bucket->numWords;
        ld    b, a
        ld    c, e  // low byte of result was saved in e
        inc   hl

        // hl: answerBuckets->byteOffsetDelta;
        // b_start += bucket->byteOffsetDelta;
        pop   de
        ld    a, e
        add   a, (hl)
        ld    e, a
        ld    a, d
        adc   a, #0x00
        ld    d, a
        inc   hl

    jr    .preload_loop$

    .preload_loop_done$:
    // Restore answer array index pointer
    // Was pushed as de
    pop   hl

    // Counter offsets for word number exit test
    inc   b
    inc   c


    // Pre-load loop counter (e gets reloaded from d)
    ld    d, #0x08

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

    // Load index into bit packed array onto stack
    push    de

    call  _getWord
    // Restore stack after pushing previous arguments
    add    sp, #2

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
