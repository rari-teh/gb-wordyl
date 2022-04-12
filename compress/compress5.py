#!/usr/bin/env python3

import sys

lang = "en"
NUM_ANSWER_BUCKETS = 15

### ENCODING SETTINGS ###

PER_LETTER_ENCODING = "5-bit"
# PER_LETTER_ENCODING = "base-26"

WORD_NUMERIC_ENCODING = "7-bit-variable"
# WORD_NUMERIC_ENCODING = "3-bit-variable"

# WORD_LETTER_ORDER = "normal"
WORD_LETTER_ORDER = "reverse"

# ALPHABET_REMAP = "normal"
ALPHABET_REMAP = "freq_of_use"



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

# Compact 3-bit + 1-bit nybble format into bytes
def dumpBlobNybbles(name, blob):
    counter_var = 0
    n = len(blob)
    # Num bytes = (nybbles / 2) rounded up
# TODO: FIXME, make accurate
    outfile.write("const uint8_t %s[%u] = {\n" % (name, ((n + 1 )/ 2 )))
    i = 0
    cur_byte = 0
    while i < n:
        # Accumulate two nybbles into each byte, then write it out
        if (i & 0x01):
            # Merge in high nybble and write out byte
            cur_byte = cur_byte | (blob[i] << 4 & 0xF0)
            outfile.write("0x%02x," % cur_byte)
            counter_var += 1
            # Line break every 16 bytes
            if (((i + 1) % 32) == 0):
                outfile.write("\n")
        else:
            cur_byte = blob[i] & 0x0F
        i+=1

    # Flush trailing byte if needed
    if ((i & 0x01) != 0x01):
        counter_var += 1
        outfile.write("0x%02x," % cur_byte)

    outfile.write("};\n\n")
    return counter_var

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
    num -= 1
    if num == 0:
        return bytes([0])
    res = []
    while num > 0:
        # 3 bit + 1 control bit encoding
        part = num & 0x7
        num = num >> 3
        if num > 0:
            part |= 0x8
        res.append(part)
    return bytes(res)

# For each word encode the numeric delta from the previous with 7 bit variable length packing
def encodeDelta_7Bit(d):
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
    input_byte_length += 5 # 5 letters per word
    buckets[ord(w[0])-ord('a')].append(w[1:])

# encoded = tuple(map(encodeList, buckets))
encoded = tuple(map(encodeList, buckets))
offsets = []
offset = 0
for e in encoded:
    offsets.append(offset)
    offset += len(e)
offsets.append(offset)

wordBlob = b''.join(encoded)

answerBits = tuple(1 if w in answerWords else 0 for w in allWords)
answerBlob = toBitmap(answerBits)

if (WORD_NUMERIC_ENCODING == "3-bit-variable"):
    dict_byte_size = dumpBlobNybbles("wordBlob", wordBlob)
else:
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

pos = 0
answerCount = 0
prevAnswerCount = 0
startBucketDelta = 0
lastBucket = 0

for i in range(NUM_ANSWER_BUCKETS):
    bucketAnswerThreshold = answerCount + bucketAnswerSize

    while (answerCount < bucketAnswerThreshold or pos%8 != 0) and pos < len(answerBits):
        if answerBits[pos]:
            answerCount += 1
        pos += 1

    # If the bucket overflowed (uncommon), create a new bucket
    if (((answerCount-prevAnswerCount) > 255) or ((startBucketDelta //8) > 255)):
        print("Error: bucket uint8 overload. Bucket:%u { %u, %u},\n" % (i, answerCount-prevAnswerCount, startBucketDelta//8))            

# TODO: FIXME
#        sys.exit()

    startBucketDelta = pos - lastBucket
    lastBucket = pos
    outfile.write("  { %u, %u},\n" % (answerCount-prevAnswerCount, startBucketDelta//8))
    # startBucket = pos
    # Store start bucket as deltas instead
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

