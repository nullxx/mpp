//
//  lexer.h
//  mpp
//
//  Created by Jon Lara trigo on 19/3/22.
//

#ifndef lexer_h
#define lexer_h

#include <stdbool.h>
#include <stdio.h>

#include "../file.h"

typedef enum {
    NO_SENTENCE_TYPE = 0,
    ARITHMETIC,
    LOGICAL,
    FLOW_CONTROL,
} SentenceType;

typedef enum { ID, PARAM } TokenType;

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

typedef enum { NO_PARAM_TYPE = 0, INM, REG, AC, REG_OR_AC, INM_OR_REG_OR_AC } ParamType;

typedef struct {
    TokenType type;
    TokenName type_name;   // only if type == ID
    ParamType param_type;  // only if type == PARAM
    char *content;
} Token;

typedef struct {
    int count;
    char *params;
} ParamT;

typedef struct {
    Token *tokens;
    int tokens_count;
    SentenceType type;
    ParamT paramT;
    RawSentence *raw;
    int row;
} Sentence;

typedef struct {
    int count;
    Sentence *sencences;
} SentenceT;

typedef struct {
    Token *token;
    ParamT *paramT;
} Sentence_template;

typedef struct {
    bool success;
    char *message;

} CheckParamsR;

SentenceT *run_lexer(RawSentenceT *rst);

#endif /* lexer_h */
