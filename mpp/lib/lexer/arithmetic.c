//
//  aritmetic.c
//  mpp
//
//  Created by Jon Lara trigo on 19/3/22.
//

#include "arithmetic.h"
#include "../file.h"
#include "../logger.h"

#include <string.h>
#include <stdio.h>

const char *TOKENS_ID[] = {
    "SUB",
    "ADD",
    "INC"
};

const int PARAM_COUNT[] = {
    2,
    2,
    0
};

bool matches_id (const char * id, char *str) {
    if (str == NULL) return false;
    const size_t str_len = strlen(str);
    if (str_len == 0) return false;

    const size_t id_size = strlen(id);
    if (str_len < id_size) return false;

    for (unsigned int i = 0; i < id_size; i++) {
        if (id[i] != str[i]) {
            return false;
        }
    }
    return true;
}


bool check_token_id(Token *token_id) {
    const int tokens_id_size = sizeof(TOKENS_ID) / sizeof(TOKENS_ID[0]);

    for (unsigned int i = 0; i < tokens_id_size; i++) {
        bool matches = matches_id(TOKENS_ID[i], token_id->content);
        if (!matches) continue;

        log_debug("Match for %s", TOKENS_ID[i]);
        
        if (strcmp(TOKENS_ID[i], "ADD") == 0) {
            token_id->name = ADD;
            return true;
        } else if (strcmp(TOKENS_ID[i], "SUB") == 0) {
            token_id->name = SUB;
            return true;
        } else if (strcmp(TOKENS_ID[i], "INC") == 0) {
            token_id->name = INC;
            return true;
        }
    }

    return false;
}

//bool get_params()
