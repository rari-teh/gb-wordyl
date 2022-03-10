#!/usr/bin/env python3

import sys

lang = "en"
NUM_ANSWER_BUCKETS = 15

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

def tobinary(w):
    s = 0
    for i in range(len(w)):
        s = (s << 5) + (ord(w[i])-ord('a'))
    return s

def encodeDelta(d):
    d-=1
    assert d<0x80*0x80*0x80
    if d < 0x80:
        return bytes((0x80|d,))
    elif d < 0x80*0x80:
        return bytes((d & 0x7F, 0x80|(d>>7)))
    else:
        return bytes((d & 0x7F, (d>>7) & 0x7F, (d>>14)))

def encodeList(ww):
    bin = tuple( map(tobinary, ww) )
    prev = 0

    md = 0

    out = b''
    for i in range(len(bin)):
        out += encodeDelta(bin[i]-prev)
        prev = bin[i]

    return out

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
    buckets[ord(w[0])-ord('a')].append(w[1:])

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

dumpBlob("wordBlob", wordBlob)
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
        sys.exit()

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

print(sum(map(len, encoded)))
#print(max(map(len, encoded)))


