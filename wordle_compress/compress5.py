#!/usr/bin/env python3

outfile = open('../src/encoded.c', 'w')

outfile.write("#include \"encoded.h\"\n\n");

def toBitmap(length, decider):
    def encodeByte(offset, decider):
        return sum( 1 << i for i in range(8) if offset+i < length and decider(offset+i) )

    return bytes(encodeByte(x, decider) for x in range(0,length,8))

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

allwords = []

with open("full.txt") as f:
    for w in f:
        w = w.strip()
        if len(w) == 5:
            allwords.append(w)

allwords.sort()
words = [[] for i in range(26)]
for w in allwords:
    words[ord(w[0])-ord('a')].append(w[1:])

encoded = tuple(map(encodeList, words))
offsets = []
offset = 0
for e in encoded:
    offsets.append(offset)
    offset += len(e)
offsets.append(offset)

wordBlob = b''.join(encoded)

special = b''
prev = 0
answers = set()
with open("answers.txt") as f:
    for w in f:
        w = w.strip()
        if len(w) == 5:
            i = allwords.index(w)
            answers.add(i)

answerBlob = toBitmap(len(allwords), lambda x : x in answers)

dumpBlob("wordBlob", wordBlob)
dumpBlob("answers", answerBlob)

# outfile.write("""typedef struct {
#  uint16_t wordNumber;
#  uint16_t blobOffset;
# } LetterList_t;

outfile.write("const LetterList_t words[27] = {\n""")

for i in range(27):
    outfile.write("  /* %s */ { %u, %u },\n" % (str(chr(ord('a')+i)) if i < 26 else "end", sum(map(len,words[:i])), offsets[i]) )

outfile.write("};\n")

outfile.close()

with open("../src/sizes.h", "w") as sizes:
    sizes.write("#define NUM_WORDS %u\n" % len(allwords))
    sizes.write("#define NUM_ANSWERS %u\n" % len(answers))

#print(sum(map(len, encoded)))
#print(max(map(len, encoded)))


