#!/usr/bin/env python3

import sys

lang = "en"
NUM_ANSWER_BUCKETS = 14
### ENCODING SETTINGS ###

PER_LETTER_ENCODING = "5-bit"
# PER_LETTER_ENCODING = "base-26"

# WORD_NUMERIC_ENCODING = "7-bit-variable"
WORD_NUMERIC_ENCODING = "3-bit-variable"

# WORD_LETTER_ORDER = "normal"
WORD_LETTER_ORDER = "reverse"

# ALPHABET_REMAP = "normal"
ALPHABET_REMAP = "freq_of_use"


ZERO_DELTA = "no-substract-by-one"
# ZERO_DELTA = "always-subtract-one"



# TODO: would be nice if these options dumped control #define flags into the rendered header file


# Optional arg 1 is language type (defaults to "en" otherwise)
# which sets input source text files and output folders under ../src/
if (len(sys.argv)) > 1:
    lang = sys.argv[1]

# output_path = "../src/"
output_path = "../src/lang_" + lang + "/"

outfile = open(output_path + "encoded.c", 'w')
outfile.write("#include \"../encoded.h\"\n\n");


def preprocessWord(w):
    # Alphabet frequency remapping
    if (ALPHABET_REMAP == "freq_of_use"):
       w = remapAlpha(w)

    # Reverse order of letters ("tiles" -> "selit")
    if (WORD_LETTER_ORDER == "reverse"):
        w = w[::-1]

    return w


def mask(n):
    # next power of 2 starting with n-i
    m = 1
    while m <= n:
        m *= 2
    return m

def toBitmap(bits):
    def encodeByte(offset):
        return sum( 1 << i for i in range(8) if offset+i < len(bits) and bits[offset+i] )

    return bytes(encodeByte(x) for x in range(0,len(bits),8))


# Compact nybbles into bytes, padding up to the nearest byte
def compactNybblestoBytes(nybbleBytes):

    # packedBytes = b''
    packedBytes = []
    n = len(nybbleBytes)
    i = 0
    cur_byte = 0
    while i < n:
        # Accumulate two nybbles into each byte, then write it out
        if (i & 0x01):
            # Merge in high nybble and write out byte
            cur_byte = cur_byte | (nybbleBytes[i] << 4 & 0xF0)
            packedBytes.append(cur_byte)
        else:
            cur_byte = nybbleBytes[i] & 0x0F
        i+=1

    # Flush trailing (low) nybble/byte if needed
    if ((i & 0x01) == 0x00):
        packedBytes.append(cur_byte)

    return bytes(packedBytes)


def dumpBlobBytes(name, blob):
    counter_var = 0
    n = len(blob)
    outfile.write("const uint8_t %s[%u] = {\n" % (name, n))
    i = 0
    while i < n:
        outfile.write("  ")
        for j in range(min(n-i,20)):
            counter_var += 1
            outfile.write("0x%02x," % blob[i])
            i+=1
        outfile.write("\n")
    outfile.write("};\n\n")
    return counter_var


def remapAlpha(source_word):
    # Reorganize letters to assign lowest values to highest frequency letters
    # remaped_alpha = "esiarnmtodlcugpkhbyfvwjzxq" # english letter frequency ranked
    # remaped_alpha = "aeorilstuncpmdbghkfwyvzjxq" # word reversed, frequency count of last 4 letters
    # remaped_alpha = "eairnoltusmdcgpkbfwvhyzxjq" # frequency count of letters 3-4 (1 based index)
    remaped_alpha = "aeioustryhkbcdfgjlmnpqvwxz" # (hackernews suggestion)
       # inverse of the above map for reverse-mapping: "almnbopjcqkrstduvhfgewxyiz"

    remapped_word = ""

    for letter in range(len(source_word)):
        for c in range(len(remaped_alpha)):
            if (source_word[letter] == remaped_alpha[c]):
                # print(source_word[letter] + " -> " + remaped_alpha[c] + "(" + str(c) + ") == " + chr(ord('a') + c))
                remapped_word += chr(ord('a') + c)
                break

    return remapped_word


# Encode each letter with 5 bits (bitpacked into 20 bytes, since first letter is dropped)
def tobinary_5bit(w):
    s = 0
    for i in range(len(w)):
        s = (s << 5) + (ord(w[i])-ord('a'))
    return s


# Encode each letter in Base 26, summed into a single value (back to front)
def tobinary_base26(w):
    s = 0
    for i, c in enumerate(w.encode('ascii')[::-1]):
        s += (c - ord('a'))*26**i
    return s


# For each word encode the numeric delta from the previous with 3 bit variable length packing
# requires later packing of nybbles into bytes
def encodeDelta_3Bit(num):
    assert num > 0
    # The delta should never be zero, so 1 bit can be saved by always subtracting 1
    if (ZERO_DELTA == "always-subtract-one"):
        num -= 1
    if num == 0:
        return bytes([0])
    res = []

    # # First count number of required varints to pack the numeric value
    # down_shift_amt = -3 # offset for first non-downshift
    # temp_num = num
    # while temp_num > 0:
    #     temp_num = temp_num >> 3
    #     down_shift_amt += 3

    # # Pack high bits first for easier decoding
    # while down_shift_amt >= 0:
    #     part = (num >> down_shift_amt) & 0x7
    #     # If more bytes will follow, set bit 4
    #     if down_shift_amt > 0:
    #         part |= 0x8
    #     res.append(part)
    #     down_shift_amt -= 3
    #
    # return bytes(res)

    # This encodes the lowest bits first, which is slower to unpack if upshifting (but not for other methods)
    while num > 0:
        # 3 bit + 1 control bit encoding
        part = num & 0x7
        num = num >> 3
        # If more bytes will follow, set bit 4
        if num > 0:
            part |= 0x8
        res.append(part)
    return bytes(res)

# For each word encode the numeric delta from the previous with 7 bit variable length packing
def encodeDelta_7Bit(d):
    # The delta should never be zero, so 1 bit can be saved by always subtracting 1
    if (ZERO_DELTA == "always-subtract-one"):
        d-=1
    assert d<0x80*0x80*0x80
    if d < 0x80:
        return bytes((0x80|d,))
    elif d < 0x80*0x80:
        return bytes((d & 0x7F, 0x80|(d>>7)))
    else:
        return bytes((d & 0x7F, (d>>7) & 0x7F, (d>>14)))

def encodeList(ww):
    if (PER_LETTER_ENCODING == "base-26"):
        bin = tuple( map(tobinary_base26, ww) )
    else:
        bin = tuple( map(tobinary_5bit, ww) )

    # Re-sorting is needed here in case the encoding changes the word numerical value sort order
    bin = sorted(bin)

    prev = 0
    md = 0

    out = b''
    for i in range(len(bin)):
        if (WORD_NUMERIC_ENCODING == "3-bit-variable"):
            out += encodeDelta_3Bit(bin[i]-prev)
        else:
            out += encodeDelta_7Bit(bin[i]-prev)
        prev = bin[i]

    # For 3-bit word encoding:
    # Squish the nybbles into bytes and round up to nearest byte at end
    # to ensure byte alignment for start of each letter bucket
    if (WORD_NUMERIC_ENCODING == "3-bit-variable"):
        out = compactNybblestoBytes(out)

    return out

input_byte_length = 0
dict_byte_size = 0
answer_bitmap_size = 0

allWords = set()
answerWords = set()

print("full_" + lang + ".txt")
print("answers_" + lang + ".txt")

with open("full_" + lang + ".txt") as f:
    for w in f:
        w = w.strip()
        if len(w) == 5:
            w = preprocessWord(w)
            allWords.add(w)

with open("answers_" + lang + ".txt") as f:
    for w in f:
        w = w.strip()
        if len(w) == 5:
            w = preprocessWord(w)
            allWords.add(w)
            answerWords.add(w)

allWords = tuple(sorted(allWords))

buckets = [[] for i in range(26)]
for w in allWords:
    # print("allwords: %u: %s" % (input_byte_length / 5, w))
    input_byte_length += 5 # 5 letters per word
    buckets[ord(w[0])-ord('a')].append(w[1:])

# encoded = tuple(map(encodeList, buckets))
encoded = tuple(map(encodeList, buckets))
offsets = []
offset = 0
for e in encoded:
    offsets.append(offset)
    offset += len(e)
    # Dump bucket array for viewing
    # res = ''.join(format(x, '02x') for x in e)
    # print (res)
    # print("* at:%u = %x" % (offset, e[len(e)-1]))
offsets.append(offset)

wordBlob = b''.join(encoded)

answerBits = tuple(1 if w in answerWords else 0 for w in allWords)
answerBlob = toBitmap(answerBits)

dict_byte_size = dumpBlobBytes("wordBlob", wordBlob)

outfile.write("// Bitmap of answers within dictionary\n")
answer_bitmap_size = dumpBlobBytes("answers", answerBlob)


# outfile.write("""typedef struct {
#   uint16_t wordNumber;
#   uint16_t blobOffset;
# } LetterList_t;
#
# const LetterList_t words[27] = {\n""")

outfile.write("const LetterBucket_t buckets[27] = {\n""")

for i in range(27):
    outfile.write("  /* %s */ { %u, %u },\n" % (str(chr(ord('a')+i)) if i < 26 else "end", sum(map(len,buckets[:i])), offsets[i]) )

outfile.write("};\n\n")

# outfile.write("""typedef struct {
#  uint16_t numWords;
#  uint16_t byteOffset;
#} AnswerBucket_t;


outfile.write("// Lookup Table to fast-forward through Answers->in->Dictionary bitmap\n")
outfile.write("// {num Answer Words, byte offset delta in Dictionary} [\n")
outfile.write("const AnswerBucket_t answerBuckets[] = {\n""")

bucketAnswerSize = len(answerWords) // NUM_ANSWER_BUCKETS
if (bucketAnswerSize > 254):
    print("Error: answer bitmap bucket size exceeds max size. Use more buckets: words(%u) / buckets(%u) = bucketsize(%u)" % (len(answerWords), NUM_ANSWER_BUCKETS, bucketAnswerSize));
    sys.exit()

dictPosPreOverflow = 255 - 8

dictPos = 0
answerCount = 0
prevAnswerCount = 0
dictByteOffsetDelta = 0
dictPosLastBucketEnd = 0


while (answerCount < len(answerWords)):
    bucketAnswerThreshold = answerCount + bucketAnswerSize

    # Break out and write a new bucket:
    #  * If there are enough answers in the bucket
    #    * Or there are close to overflowing the max (byte) bitmap offset size per bucket
    #  * But only on even answer bitmap byte boundaries
    #  * And as long as the dict count is within the bitmap size
    while (((answerCount < bucketAnswerThreshold) and ((dictByteOffsetDelta//8) < dictPosPreOverflow))
            or dictPos%8 != 0) and dictPos < len(answerBits):
        if answerBits[dictPos]:
            answerCount += 1
        dictPos += 1
        dictByteOffsetDelta = dictPos - dictPosLastBucketEnd

    if ((dictByteOffsetDelta//8) >= dictPosPreOverflow):
        print("Note: bucket close to overflow, adding another. Bucket: %u { numAnswers: %u, dictByteOffset: %u},\n" % (i, answerCount-prevAnswerCount, dictByteOffsetDelta//8))
    dictPosLastBucketEnd = dictPos
    outfile.write("  { %u, %u},\n" % (answerCount-prevAnswerCount, dictByteOffsetDelta//8))
    dictByteOffsetDelta = 0
    prevAnswerCount = answerCount

outfile.write("};\n")

outfile.close()

with open(output_path + "sizes.h", "w") as sizes:
    sizes.write("#define NUM_WORDS %u\n" % len(allWords))
    sizes.write("#define NUM_ANSWERS %u\n" % len(answerWords))
    sizes.write("#define NUM_ANSWER_BUCKETS %u\n" % NUM_ANSWER_BUCKETS)
    sizes.write("#define NUM_ANSWERS_ROUNDED_UP_POW2 %u" % mask(len(answerWords)))

print ("Input size: " + str(input_byte_length) + ", Dict out size: " + str(dict_byte_size) + ", Answer Bitmap size: " + str(answer_bitmap_size));
# print(sum(map(len, encoded)))
#print(max(map(len, encoded)))


#  Get un-map version of remap alphabet string
# str_remap = "abcdefghijklmnopqrstuvwxyz"
# str_remap = remapAlpha(str_remap)
# print (str_remap)
