//
//  lexer.c
//  mpp
//
//  Created by Jon Lara trigo on 19/3/22.
//

#include "lexer.h"
#include "arithmetic.h"
#include "../error.h"

#include <string.h>
#include <stdlib.h>
#include <math.h>

bool get_id_token(Token *token_id) {
    bool is_arithmetic = check_token_id(token_id);
    if (is_arithmetic) return true;
    return false;
}

Token *get_tokens(RawSentence rs, int *count) {
    Token *tokens = malloc(sizeof(Token) * 0);
    char *token = strtok(rs.content, " "); // "       SOMETHING" => { "SOMETHING" } // It does like a trim

    unsigned i = 0;
    while (token != NULL) {
        tokens = realloc(tokens, sizeof(Token) * (i + 1));
        tokens[i].type = i == 0 ? ID : PARAM;
        
        tokens[i].content = malloc(sizeof(char) * strlen(token) + 1);
        strcpy(tokens[i].content, token);

        token = strtok(NULL, " ");

        (*count)++;
        i++;
    }

    return tokens;
}

void process_line(RawSentence rs) {
    Error err = {0};
    Sentence st = {0};
    
    // get all the tokens
    int tokens_size = 0;
    Token *tokens = get_tokens(rs, &tokens_size);
    st.tokens = tokens;

    // find the token_id
    Token *token_id = NULL;
    for (unsigned int i = 0; i < tokens_size; i++) {
        if (tokens[i].type == ID) {
            token_id = &tokens[i];
            break;
        }
    }
    

    if (token_id == NULL) {
        err.type = FATAL;
        const char *detail_message = "Couldn't find token (id) at line %d\n";

        const size_t detail_message_len = strlen(detail_message);
        const int row_num_len = (int) log10(rs.row + 1) + 1;

        char *message = malloc(sizeof(char) * (detail_message_len + row_num_len) - 2*1);

        sprintf(message, detail_message, rs.row + 1);

        err.message = message;
        err.show_errno = false;
        goto exception;
    }

    

    bool id_token_found = get_id_token(token_id);
    if (!id_token_found) {
        err.type = FATAL;
        const char *detail_message = "Couldn't parse token (id) '%s' at line %d\n";

        const size_t detail_message_len = strlen(detail_message);
        const int row_num_len = (int) log10(rs.row + 1) + 1;
        const size_t token_len = strlen(token_id->content);

        char *message = malloc(sizeof(char) * (detail_message_len + row_num_len + token_len) - 2*2);

        sprintf(message, detail_message, token_id->content, rs.row + 1);

        err.message = message;
        err.show_errno = false;
        goto exception;
    }


    free(st.tokens);
    // TODO free(st.tokens[0..n].content)
    free(st.paramT.params);
    free(st.raw);

    free(rs.content);
    return;
exception:
    process_error(&err);
        
}

void run_lexer(RawSentenceT *rst) {
    for (unsigned int i = 0; i < rst->count; i++) {
        process_line(rst->raw_sentences[i]);
    }
}
