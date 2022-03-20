//
//  lexer.h
//  mpp
//
//  Created by Jon Lara trigo on 19/3/22.
//

#ifndef lexer_h
#define lexer_h

#include <stdio.h>
#include "../file.h"

typedef enum {
    ARITHMETIC,
    LOGICAL,
    FLOW_CONTROL,
} SentenceType;

typedef enum {
    ID,
    PARAM
} TokenType;

typedef enum {
    MOV,
    SUB,
    CMP,
    AND,
    OR,
    XOR,
    INC,
    ADD,
} TokenName;

typedef struct {
    TokenType type;
    TokenName name; // only if type == ID
    char *content;
} Token;

typedef struct {
    unsigned int count;
    char *params;
} ParamT;

typedef struct {
    Token *tokens;
    SentenceType type;
    unsigned int params_count;
    ParamT paramT;
    RawSentence *raw;
} Sentence;

void run_lexer(RawSentenceT *rst);

#endif /* lexer_h */
