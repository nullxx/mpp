//
//  file.h
//  mpp
//
//  Created by Jon Lara trigo on 19/3/22.
//

#ifndef file_h
#define file_h

typedef struct {
    int row;
    char *content;
} RawSentence;

typedef struct {
    unsigned int count;
    RawSentence *raw_sentences;
} RawSentenceT;

void read_file(const char *path, RawSentenceT *raw_sentence_t);

#endif /* file_h */
