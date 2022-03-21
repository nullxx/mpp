//
//  lexer.c
//  mpp
//
//  Created by Jon Lara trigo on 19/3/22.
//

#include "lexer.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "../error.h"
#include "arithmetic.h"

SentenceType get_id_token(Token *token_id) {
    bool is_arithmetic = check_arithmetic_token_id(token_id);
    if (is_arithmetic) return ARITHMETIC;
    return NO_SENTENCE_TYPE;
}

CheckParamsR check_params(TokenName token_id_name, Sentence st) {
    CheckParamsR cpr = {false, NULL};

    cpr = check_arithmetic_params(token_id_name, st);
    if (cpr.success) return cpr;

    return cpr;
}

Token *get_tokens(RawSentence *rs, int *count) {
    Token *tokens = malloc(sizeof(Token) * 0);
    char *token = strtok(rs->content, " ");  // "       SOMETHING" => { "SOMETHING" } // It does like a trim

    unsigned i = 0;
    while (token != NULL) {
        tokens = realloc(tokens, sizeof(Token) * (i + 1)); // FIXME tokens nulled but not freed
        tokens[i].type = i == 0 ? ID : PARAM;

        tokens[i].content = malloc(sizeof(char) * strlen(token) + 1);
        strcpy(tokens[i].content, token);

        token = strtok(NULL, " ");

        (*count)++;
        i++;
    }

    return tokens;
}

Sentence process_line(RawSentence *rs) {
    Error err = {0};
    Sentence st = {0};

    st.row = rs->row;

    st.raw = rs;
    // get all the tokens
    Token *tokens = get_tokens(rs, &st.tokens_count);
    st.tokens = tokens;

    // find the token_id
    Token *token_id = NULL;
    if (st.tokens_count > 0 && st.tokens[0].type == ID) token_id = &st.tokens[0];

    if (token_id == NULL) {
        err.type = FATAL;
        const char *detail_message = "Couldn't find token (id) at line %d\n";

        const size_t detail_message_len = strlen(detail_message);
        const int row_num_len = (int)log10(rs->row + 1) + 1;

        char *message = malloc(sizeof(char) * (detail_message_len + row_num_len) - 2 * 1);
        sprintf(message, detail_message, rs->row + 1);

        err.message = message;
        err.show_errno = false;
        goto exception;
    }

    SentenceType sen_type = get_id_token(token_id);
    if (sen_type == NO_SENTENCE_TYPE) {
        err.type = FATAL;
        const char *detail_message = "Unexpected token (ID) '%s' at line %d\n";

        const size_t detail_message_len = strlen(detail_message);
        const int row_num_len = (int)log10(rs->row + 1) + 1;
        const size_t token_len = strlen(token_id->content);

        char *message = malloc(sizeof(char) * (detail_message_len + row_num_len + token_len) - 2 * 2);

        sprintf(message, detail_message, token_id->content, rs->row + 1);

        err.message = message;
        err.show_errno = false;
        goto exception;
    }
    st.type = sen_type;

    CheckParamsR cpr = check_params(token_id->type_name, st);
    if (!cpr.success) {
        err.type = FATAL;

        err.message = cpr.message;
        err.show_errno = false;
        goto exception;
    }

    //    free(st.tokens);
    //    // TODO free(st.tokens[0..n].content)
    //    free(st.paramT.params);
    //    free(rs->content);
    return st;
exception:
    process_error(&err);
    return st;
}

SentenceT *run_lexer(RawSentenceT *rst) {
    SentenceT *sentencest = malloc(sizeof(SentenceT));
    sentencest->sencences = malloc(sizeof(Sentence) * rst->count);
    sentencest->count = rst->count;
    for (unsigned int i = 0; i < rst->count; i++) {
        sentencest->sencences[i] = process_line(&rst->raw_sentences[i]);
    }
    return sentencest;
}
