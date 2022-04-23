
#include <stdint.h>

#ifndef _ENCODED_H
#define _ENCODED_H

extern const uint8_t wordBlob[];

extern const uint8_t answers[];

typedef struct {
  uint16_t wordNumber;
  uint16_t blobOffset;
} LetterBucket_t;

typedef struct {
  uint16_t   wordCount;
  uint16_t   blobOffset;
  uint8_t    firstLetter;
  uint32_t   wordVal;
} RAMLetterBucket_t;


extern const LetterBucket_t buckets[];

typedef struct {
  uint8_t numWords;
  uint8_t byteOffsetDelta;
} AnswerBucket_t;

extern const AnswerBucket_t answerBuckets[];


#endif
