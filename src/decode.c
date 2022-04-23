#include <stdint.h>
#include <stdbool.h>
#include <stdbool.h>

#include <asm/types.h>
#include "encoded.h"
#include "sizes.h"


#define _ASM_GETSPECIALWORD

// #define _ASM_UPDATEWORD
#define _UPDATEWORD_3BIT_VARINT

// #define ALPHABET_REMAP
// #define WORD_LETTERS_REVERSED

// #define NO_ZERO_DELTA_SUBTRACT
#define YES_ZERO_DELTA_SUBTRACT


static uint32_t currentWord;
static const uint8_t* blobPtr;
static char  * str_return_buffer;


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

#elif defined (_UPDATEWORD_3BIT_VARINT)


bool dict_two_nybbles_queued;
uint8_t dict_cur_byte;

/*// union for more efficient ORing in of lowest byte
typedef union _wordyl_u32 {
    struct {
        uint8_t b0;
        uint8_t b1;
        uint8_t b2;
        uint8_t b3;
    };
    struct {
        uint8_t b0;
        uint8_t b1;
        uint8_t b2;
        uint8_t b3;
    } b;
    uint32_t ul;
} _wordyl_u32;

// uint32_t update_v;
_wordyl_u32 update_v;


// TODO: ASM version, it will probably go 2x - 4x faster at least


// dict_two_nybbles_queued and dict_cur_byte need to be
// primed once at the start of the decode loop
void updateWord_3bit_varint(void) OLDCALL {

    //uint32_t v = 0;
    // Add 1 since all words are encoded as (value - 1)
    update_v.ul = 0; //1;
    bool loop_done = false;

    // Merge in 3 bit varints until the "more nybbles" flag is not set
    do {
    // while (1) {
        update_v.ul <<= 3; // TODO: wasteful to do this for every one
        update_v.b.b0 |= (uint32_t)(dict_cur_byte & 0x07);

        // Exit loop (after loading next byte/nybble)
        // if this is the last varint nybble for the word
        if ((dict_cur_byte & 0x08) == 0)
            loop_done = true;

        // If a nybble is queued then move it into the lower 4 bits
        // Otherwise read a new byte
        if (dict_two_nybbles_queued)
            dict_cur_byte >>= 4;
        else
            dict_cur_byte = *blobPtr++;

        dict_two_nybbles_queued = !dict_two_nybbles_queued;
    // };
    } while (!loop_done);

    // Add 1 since all words are encoded as (value - 1)
    #ifdef YES_ZERO_DELTA_SUBTRACT
        currentWord += update_v.ul + 1;
    #else
        currentWord += update_v.ul;
    #endif
}
*/

/*
// V2
uint32_t update_v;

// dict_two_nybbles_queued and dict_cur_byte need to be
// primed once at the start of the decode loop
void updateWord_3bit_varint(void) OLDCALL {

    update_v = 0;
    uint8_t n_count = 0;
    bool loop_done = false;

    // Merge in 3 bit varints until the "more nybbles" flag is not set
    do {

        uint32_t merge_bits = (dict_cur_byte & 0x07);
        for (uint8_t c = 0; c < n_count; c++)
            merge_bits <<= 3;

        update_v |= merge_bits;

        // Exit loop (after loading next byte/nybble)
        // if this is the last varint nybble for the word
        if ((dict_cur_byte & 0x08) == 0)
            loop_done = true;

        n_count++;

        // If a nybble is queued then move it into the lower 4 bits
        // Otherwise read a new byte
        if (dict_two_nybbles_queued)
            dict_cur_byte >>= 4;
        else
            dict_cur_byte = *blobPtr++;

        dict_two_nybbles_queued = !dict_two_nybbles_queued;
    // };
    } while (!loop_done);

    // Add 1 since all words are encoded as (value - 1)
    #ifdef YES_ZERO_DELTA_SUBTRACT
        currentWord += update_v + 1;
    #else
        currentWord += update_v;
    #endif
}
*/

// V3 - expects lowest bits (chunks) to be packed first, highest last
uint32_t update_v;
bool updateword_loop_done;

// dict_two_nybbles_queued and dict_cur_byte need to be
// primed once at the start of the decode loop
void updateWord_3bit_varint(void) OLDCALL {

    update_v = 0;
    uint8_t * p_num = (uint8_t *)&update_v;

    uint8_t n_count = 0;
    //bool loop_done = false;
    updateword_loop_done = false;

    // Merge in 3 bit varints until the "more nybbles" flag is not set
    do {
        switch (n_count) {
                    // [0]2..0
            case 0: *p_num |= (dict_cur_byte & 0x07);
                    break;

                    // [0]bits 5..3
            case 1: *p_num |= ((dict_cur_byte & 0x07) << 3);
                    break;

                    // [0]7..6 , [1]..0
            case 2: *p_num++ |= (dict_cur_byte << 6);
                    *p_num |= ((dict_cur_byte & 0x07) >> 2);
                    break;

                    // [1]3..1
            case 3: *p_num |= ((dict_cur_byte & 0x07) << 1);
                    break;

                    // [1]6..4
            case 4: *p_num |= ((dict_cur_byte & 0x07) << 4);
                    break;

                    // [1]7 , [2]1..0
            case 5: *p_num++ |= (dict_cur_byte << 7);
                    *p_num |= ((dict_cur_byte & 0x07) >> 1);
                    break;

                    // [2]bits 4..2
            case 6: *p_num |= ((dict_cur_byte & 0x07) << 2);
                    break;

                    // [2]bits 7..5
            case 7: *p_num |= ((dict_cur_byte & 0x07) << 5);
                    break;

            // Only need to support decode up to 20 bits
            // Apparently commenting out below de-prioritizes using BC for delta_v
            // Which makes the code 200 bytes larger as well as slower. So it stays :)

                    // [3]bits 2..0
            case 8: p_num++;
                    *p_num |= (dict_cur_byte & 0x07);
                    break;

                    // [3]bits 5..3
            case 9: *p_num |= ((dict_cur_byte & 0x07) << 3);
                    break;

                    // [3]7..6
            case 10: *p_num |= (dict_cur_byte << 6);
                    break;


        }

        // Exit loop (after loading next byte/nybble)
        // if this is the last varint nybble for the word
        if ((dict_cur_byte & 0x08) == 0)
            updateword_loop_done = true;

        n_count++;

        // If a nybble is queued then move it into the lower 4 bits
        // Otherwise read a new byte
        if (dict_two_nybbles_queued)
            dict_cur_byte >>= 4;
        else
            dict_cur_byte = *++blobPtr;

        dict_two_nybbles_queued = !dict_two_nybbles_queued;
    // };
    } while (!updateword_loop_done);

    // Add 1 since all words are encoded as (value - 1)
    #ifdef YES_ZERO_DELTA_SUBTRACT
        currentWord += update_v + 1;
    #else
        currentWord += update_v;
    #endif
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

/*
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

    #ifdef _UPDATEWORD_3BIT_VARINT
        // For 3-bit encoding the first nybble in a given bucket
        // is padded to always align in the low nybble of the first byte
        dict_cur_byte = *blobPtr; //++;
        dict_two_nybbles_queued = true;
    #endif

    for (uint16_t j = n - w->wordNumber + 1; j; j--) {
        #ifdef _UPDATEWORD_3BIT_VARINT
            updateWord_3bit_varint();
        #else
            updateWord();
        #endif
    }
    decodeWord(i, currentWord, str_return_buffer);
}
*/


#ifdef _UPDATEWORD_3BIT_VARINT

#define DICT_LUT_RAM_CHUNK_SIZE    500u // 250u = 67,000 cycles, 500 = 67,000, 1000 = 508,000
#define DICT_LUT_RAM_ALPHA_BUCKETS (26 + 1)
// Sets a conservative maximum number of RAM buckets
#define DICT_LUT_RAM_BUCKETS_MAX ((NUM_WORDS / DICT_LUT_RAM_CHUNK_SIZE) + (DICT_LUT_RAM_ALPHA_BUCKETS))

#define DICT_BUCKETS_EOF  0xFFu


RAMLetterBucket_t dictLUT_RAM[DICT_LUT_RAM_BUCKETS_MAX];
RAMLetterBucket_t * p_dictLUT_RAM;
uint8_t dictLUT_RAM_count;
uint16_t bucketWordCount;
uint16_t bucketStart_BlobOffset;
uint32_t bucketStart_currentWord;


void addRAMBucket(char first_letter) {
    p_dictLUT_RAM->firstLetter = first_letter;
    p_dictLUT_RAM->blobOffset = bucketStart_BlobOffset;
    p_dictLUT_RAM->wordCount = bucketWordCount;
    p_dictLUT_RAM->wordVal = bucketStart_currentWord;

    // Move to next bucket and reset word count
    bucketWordCount = 0;
    dictLUT_RAM_count++;
    p_dictLUT_RAM++;

    bucketStart_BlobOffset = (uint16_t)(blobPtr - wordBlob);
    bucketStart_currentWord = currentWord;
}


// Current version takes about 3 seconds


// Builds a RAM lookup table to make 3-bit encoding speed viable
void buildRAMBuckets(void) {

    uint8_t start_letter;

    const LetterBucket_t* p_dictLUT_ROM = buckets; // Dictionary first-letter buckets

    dictLUT_RAM_count = 0;
    p_dictLUT_RAM = dictLUT_RAM;


    // Loop through each dictionary Letter Bucket
    for (start_letter = 0 ; start_letter < 26; start_letter++) {

        // For each letter bucket the cumulative wordvalue gets reset to 0
        // Since the initial delta in a first alpha bucket is the whole word value
        currentWord = 0;

        bucketWordCount = 0;
        blobPtr = wordBlob + p_dictLUT_ROM->blobOffset;
        // Save starting offset for current bucker, since pointer gets incremented
        bucketStart_BlobOffset = (uint16_t)(blobPtr - wordBlob);
        bucketStart_currentWord = currentWord;

            // For 3-bit encoding the first nybble in a given bucket
            // is padded to always align in the low nybble of the first byte
            dict_cur_byte = *blobPtr; // ++;
            dict_two_nybbles_queued = true;

        uint16_t curSizeRAMBucket = 0;
        // Number of words is in next bucket
        for (uint16_t curBucketCount = ((p_dictLUT_ROM + 1)->wordNumber - p_dictLUT_ROM->wordNumber); curBucketCount > 0; curBucketCount--) {
            updateWord_3bit_varint();
            bucketWordCount++;
            curSizeRAMBucket++;

            // Add a new RAM bucket every N words
            if (curSizeRAMBucket >= DICT_LUT_RAM_CHUNK_SIZE)

                // Only create new RAM bucket if it's first word can be evenly aligned on a byte boundary
                // i.e. it's first nybble is in the LSbits
                if (dict_two_nybbles_queued) {
                    addRAMBucket(start_letter);
                    curSizeRAMBucket = 0;
                }
        }
        // Save any trailing words if needed
        addRAMBucket(start_letter);

        // Move to next letter bucket
        p_dictLUT_ROM++;
    }

    // Add final terminating entry
    bucketStart_currentWord = 0;
    addRAMBucket(DICT_BUCKETS_EOF);
}



void loadDictBucketInitVals(void) {
    // For each letter bucket the delta gets reset to 0
    currentWord = p_dictLUT_RAM->wordVal;
    blobPtr = wordBlob + p_dictLUT_RAM->blobOffset;

    // For 3-bit encoding the first nybble in a given bucket
    // is padded to always align in the low nybble of the first byte
    dict_cur_byte = *blobPtr; // ++;
    dict_two_nybbles_queued = true;
}

// getWord RAM VERSION

// Gets an answer word (from the dictionary) by index number in
// the answer list
void getWord(uint16_t dictWordNum) {

    // Default to failed to find the requested word
    *str_return_buffer = 0;

    // Validate requested word is within range
    if (dictWordNum > (NUM_WORDS - 1))
        return; // failed: str_return_buffer will be NULLED

    dictWordNum++; // make the word number 1 based instead of zero based

    // Use RAM bucket index to fast-forward closer to desired answer word
    p_dictLUT_RAM = dictLUT_RAM;

    while (dictWordNum > p_dictLUT_RAM->wordCount) {
        dictWordNum -= p_dictLUT_RAM->wordCount;
        p_dictLUT_RAM++;

        if (p_dictLUT_RAM->firstLetter == (DICT_BUCKETS_EOF))
            return; // failed: str_return_buffer will be NULLED
    }

    loadDictBucketInitVals();

    for (uint16_t j = dictWordNum; j; j--) {
        updateWord_3bit_varint();
    }
    decodeWord(p_dictLUT_RAM->firstLetter, currentWord, str_return_buffer);
}



// Encodes guess word to 5-bits per letter, then seeks through
// dictionary looking for the numeric match
// Returns true if matched or false (0) if no match was found
uint8_t filterWord(char* s) {
    // because the input system uses an on-screen keyboard with A-Z only, no need to sanitize
    // for (i=0; i<5; i++)
    //    if (s[i] < 'A' || s[i] > 'Z')
    //        return 0;
    uint32_t userWordVal = 0;

    #ifdef WORD_LETTERS_REVERSED
        int8_t letter;
        // Words encoded with letters in reverse order
        for (letter=3;letter>=0;letter--)
            userWordVal = (userWordVal << 5) | (check_alpha_remap_char(s[letter])-'A');

        uint8_t first_letter = check_alpha_remap_char(s[4])-'A';
    #else
        uint8_t letter;
        // Words encoded with normal letter order
        for (letter=1;letter<5;letter++)
            userWordVal = (userWordVal << 5) | (check_alpha_remap_char(s[letter])-'A');

        uint8_t first_letter = check_alpha_remap_char(s[0])-'A';
    #endif


    // Use RAM bucket index to fast-forward closer to desired answer word
    p_dictLUT_RAM = dictLUT_RAM;

    // Scan for first bucket with matching initial letter
    while (first_letter != p_dictLUT_RAM->firstLetter) {
        p_dictLUT_RAM++;

        if (p_dictLUT_RAM->firstLetter == (DICT_BUCKETS_EOF))
            return 0;
    }

    // Loop through buckets until either the first letter doesn't match
    // or the user wordVal is less than the bucket start wordVal
    // Note: For each initial alpha bucket the wordVal resets to 0
    while ((first_letter == p_dictLUT_RAM->firstLetter) &&
           (userWordVal >= p_dictLUT_RAM->wordVal)) {

        p_dictLUT_RAM++;
    }
    // Now back up one since the above loop is designed to overshoot by +1
    // since it can't check if a wordVal is within a single bucket since
    // buckets don't store a "maxWordValue"
    p_dictLUT_RAM--;

    loadDictBucketInitVals();

    // Scan through entire bucket decoding words to see if there is a match
    for (uint16_t j = p_dictLUT_RAM->wordCount; j; j--) {
        updateWord_3bit_varint();
        if (currentWord >= userWordVal)
            return currentWord == userWordVal;
    }

    return 0;
}




#endif // _UPDATEWORD_3BIT_VARINT


/*// Check to see if the guess word is in the dictionary.
//
// Encodes guess word to 5-bits per letter, then seeks through
// dictionary looking for the numeric match
uint8_t filterWord(char* s) {
    // because the input system uses an on-screen keyboard with A-Z only, no need to sanitize
    // for (i=0; i<5; i++)
    //    if (s[i] < 'A' || s[i] > 'Z')
    //        return 0;
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

    #ifdef _UPDATEWORD_3BIT_VARINT
        // For 3-bit encoding the first nybble in a given bucket
        // is padded to always align in the low nybble of the first byte
        dict_cur_byte = *blobPtr; //++;
        dict_two_nybbles_queued = true;
    #endif

    for (uint16_t j=buckets[i+1].wordNumber - buckets[i].wordNumber; j; j--) {
        #ifdef _UPDATEWORD_3BIT_VARINT
            updateWord_3bit_varint();
        #else
            updateWord();
        #endif

        if (currentWord >= w) {
            return currentWord == w;
        }
    }

    return 0;
}
*/

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
