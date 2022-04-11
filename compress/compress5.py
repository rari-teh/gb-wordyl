#!/usr/bin/env python3

import sys

lang = "en"
NUM_ANSWER_BUCKETS = 15

# * Letter encoding             <- lower computational and complexity cost
# * Word numeric value encoding <- higher complexity and cost (unpacking bytes)
# * Reverse letters             <- should be low cost to unpack
# * Remapped alpha weighting    <- should be low cost to unpack/translate
#      - Change a-z letter encoding to weight low and high frequencies strategically
#       - could calc per dictionary, skipping first letters of words
#       - english: https://en.wikipedia.org/wiki/Letter_frequency
#         alpha_lookup = "esiarnmtodlcugpkhbyfvwjzxq"  # e = most frequent, q = least
#       - http://www.norvig.com/mayzner.html
# TODO:
#   * 7 bit -> allicate unused bits for most common 2 or 3 letter sequences
#       -  (calc per dict)
#       -  Two-Letter Sequence (Bigram) Counts
#           http://www.norvig.com/mayzner.html
#   * Use word bit encoding "prefixes" (see notes)



# Per-letter numeric encoding
#  * 5-bit: 5 bits / letter
#  * b-26:  Base 26 letters

# Calculated word numeric value encoding
# * 7-bit: 7 bit varliable encoding
# * 7-bit: 3 bit varliable encoding

# Word letter order
#  * Norm: Normal letter order (bakes -> bakes)
#  * Rev:  Reversed letter order (bakes -> sekab)

# Alphabet weighting
#  * Norm: Normal alphabet order (abcde..xyz)
#  * Freq-1: Assign highest frequency letters the lowest value/first place in alphabet "esiarnmtodlcugpkhbyfvwjzxq"
#  * Freq-2: Assign highest frequency letters the lowest value/first place in alphabet "aeioustryhkbcdfgjlmnpqvwxz"
#            - hackernews

# Letter Wordval W-Ord Alpha
# 5-bit  7-bit                :  17,757
# 5-bit  3-bit                :  16,315
# B-26   3-bit                :  15,631
# B-26   7-bit  W-Rev         :  14,856.5  *  <- has bitmap 1 bucket overflow (TODO) <-- ERROR?
# B-26   3-bit  W-Rev         :  14,304       <- has bitmap 1 bucket overflow (TODO)
# B-26   3-bit  W-Rev  Freq-1 :  13,632.0     <- has bitmap 1 bucket overflow (TODO)
# B-26   3-bit  W-Rev  Freq-2 :  13,253.0     <- has bitmap 1 bucket overflow (TODO)

# B-26   7-bit  W-Rev  Freq-2 :  16,248       <- has bitmap 1 bucket overflow (TODO)



# Optional arg 1 is language type (defaults to "en" otherwise)
# which sets input source text files and output folders under ../src/
if (len(sys.argv)) > 1:
	lang = sys.argv[1]

# output_path = "../src/"
output_path = "../src/lang_" + lang + "/"

outfile = open(output_path + "encoded.c", 'w')
outfile.write("#include \"../encoded.h\"\n\n");

def preprocessWord(w):
    return w
    # try: w[::-1]

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

def dumpBlobNybbles(name, blob):
    n = len(blob)
    # Num bytes = (nybbles / 2) rounded up
    outfile.write("const uint8_t %s[%u] = {\n" % (name, ((n + 1 )/ 2 )))
    i = 0
    cur_byte = 0
    while i < n:
        # Accumulate two nybbles into each byte, then write it out
        if (i & 0x01):
            # Merge in high nybble and write out byte
            cur_byte = cur_byte | (blob[i] << 4 & 0xF0)
            outfile.write("0x%02x," % cur_byte)
            # Line break every 16 bytes
            if (((i + 1) % 32) == 0):
                outfile.write("\n")
        else:
            cur_byte = blob[i] & 0x0F
        i+=1

    # Flush trailing byte if needed
    if ((i & 0x01) != 0x01):
        outfile.write("0x%02x," % cur_byte)

    outfile.write("};\n\n")

def dumpBlob(name, blob):
    n = len(blob)
    outfile.write("const uint8_t %s[%u] = {\n" % (name, n))
    i = 0
    while i < n:
        outfile.write("  ")
        for j in range(min(n-i,20)):
            outfile.write("0x%02x," % blob[i])
            i+=1
        outfile.write("\n")
    outfile.write("};\n\n")


def remapAlpha(source_word):
    # TEST: reorganize letters to assign lowest values to highest frequency letters
    # remaped_alpha = "esiarnmtodlcugpkhbyfvwjzxq" # v1
    remaped_alpha = "aeioustryhkbcdfgjlmnpqvwxz" # v2 (hackernews)
    remapped_word = ""

    for letter in range(len(source_word)):
        for c in range(len(remaped_alpha)):
            if (source_word[letter] == remaped_alpha[c]):
                print(source_word[letter] + " -> " + remaped_alpha[c] + "(" + str(c) + ") == " + chr(ord('a') + c))
                remapped_word += chr(ord('a') + c)
                break

    return remapped_word


# Reversing letters before encoding might save additional couple hundred bytes
  # Have to reverse sort each word coming in (answers + dict)
    # Then re-sort the lists first
def tobinary_5bit(w):

    # Alphabet frequency remapping
    # w = remapAlpha(w)

    # 5 bit encoding per letter
    print(w + "...")
    s = 0
    for i in range(len(w)):
        s = (s << 5) + (ord(w[i])-ord('a'))
    print(w + " = " + str(s))
    return s

def tobinary_b26(w):

    # w = remapAlpha(w)

    print(w + "...")
    s = 0
    # TEST: Base 26 encoding instead of 5 bit per letter packed encoding
    for i, c in enumerate(w.encode('ascii')[::-1]):
        s += (c - ord('a'))*26**i

    print(w + " = " + str(s))
    return s


# requires later packing of nybbles into bytes
def encodeDelta_3Bit(num):
    assert num > 0
    num -= 1
    if num == 0:
        return bytes([0])
    res = []
    while num > 0:
        # # 7 bit + 1 encoding
        # part = num & 0x7F
        # num = num >> 7
        # if num > 0:
        #     part |= 0x80

        # 3 bit + 1 encoding
        part = num & 0x7
        num = num >> 3
        if num > 0:
            part |= 0x8
        res.append(part)
    return bytes(res)

# 7-bit word numeric encoding
def encodeDelta_7Bit(d):
    assert d<0x80*0x80*0x80
    if d < 0x80:
        return bytes((0x80|d,))
    elif d < 0x80*0x80:
        return bytes((d & 0x7F, 0x80|(d>>7)))
    else:
        return bytes((d & 0x7F, (d>>7) & 0x7F, (d>>14)))

def encodeList(ww):
    # bin = tuple( map(tobinary, ww) )
    bin = tuple( map(tobinary_5bit, ww) )
    # bin = tuple( map(tobinary_b26, ww) )
    # bin = tuple( sorted(map(tobinary, ww)) )
    print("--------------------------------------------------")
    prev = 0

    md = 0

    out = b''
    bin = sorted(bin)
#    for i in range(len(bin)):
#        print(bin[i])
    for i in range(len(bin)):
        # out += encodeDelta(bin[i]-prev)
        out += encodeDelta_7Bit(bin[i]-prev)
        # out += encodeDelta_3Bit(bin[i]-prev)
        prev = bin[i]

    print("#################################################")
    return out

allWords = set()
answerWords = set()

print("full_" + lang + ".txt")
print("answers_" + lang + ".txt")

with open("full_" + lang + ".txt") as f:
    for w in f:
        w = w.strip()
        # reverse order of letters
        # w = w[::-1]
        if len(w) == 5:
            w = preprocessWord(w)
            allWords.add(w)

with open("answers_" + lang + ".txt") as f:
    for w in f:
        w = w.strip()
        # reverse order of letters
        # w = w[::-1]
        if len(w) == 5:
            w = preprocessWord(w)
            allWords.add(w)
            answerWords.add(w)


allWords = tuple(sorted(allWords))

buckets = [[] for i in range(26)]
for w in allWords:
    buckets[ord(w[0])-ord('a')].append(w[1:])

encoded = tuple(map(encodeList, buckets))

# 7bit word value encoding
print(sum(map(len, encoded)))
# 3bit encoding
# print( ( sum(map(len, encoded) )  / 2) )



offsets = []
offset = 0
for e in encoded:
    offsets.append(offset)
    offset += len(e)
offsets.append(offset)

wordBlob = b''.join(encoded)

answerBits = tuple(1 if w in answerWords else 0 for w in allWords)
answerBlob = toBitmap(answerBits)

# dumpBlob("wordBlob", wordBlob)
dumpBlobNybbles("wordBlob", wordBlob)

dumpBlob("answers", answerBlob)

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

outfile.write("const AnswerBucket_t answerBuckets[] = {\n""")

targetSize = len(answerWords) // NUM_ANSWER_BUCKETS

pos = 0
count = 0
prevCount = 0
startBucketDelta = 0
lastBucket = 0

for i in range(NUM_ANSWER_BUCKETS):
    targetCount = count + targetSize
    while (count < targetCount or pos%8 != 0) and pos < len(answerBits):
        if answerBits[pos]:
            count += 1
        pos += 1
    if (((count-prevCount) > 255) or ((startBucketDelta //8) > 255)):
        print("Error: bucket uint8 overload. Bucket:%u { %u, %u},\n" % (i, count-prevCount, startBucketDelta//8))
        # TODO: Fixme
        # sys.exit()

    startBucketDelta = pos - lastBucket
    lastBucket = pos
    outfile.write("  { %u, %u},\n" % (count-prevCount, startBucketDelta//8))
    # startBucket = pos
    # Store start bucket as deltas instead
    prevCount = count
outfile.write("};\n")

outfile.close()

with open(output_path + "sizes.h", "w") as sizes:
    sizes.write("#define NUM_WORDS %u\n" % len(allWords))
    sizes.write("#define NUM_ANSWERS %u\n" % len(answerWords))
    sizes.write("#define NUM_ANSWER_BUCKETS %u\n" % NUM_ANSWER_BUCKETS)
    sizes.write("#define NUM_ANSWERS_ROUNDED_UP_POW2 %u" % mask(len(answerWords)))

#print(max(map(len, encoded)))


