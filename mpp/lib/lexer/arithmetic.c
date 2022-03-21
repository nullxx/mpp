//
//  aritmetic.c
//  mpp
//
//  Created by Jon Lara trigo on 19/3/22.
//

#include "arithmetic.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../file.h"
#include "../logger.h"

const char *TOKENS_ID[] = {"SUB", "ADD", "INC"};

Token SUB_token_template = {ID, SUB, NO_PARAM_TYPE, "SUB"};
ParamT SUB_paramt_template = {1, NULL};
Sentence_template SUB_template_sentence = {&SUB_token_template, &SUB_paramt_template};

Token ADD_token_template = {ID, ADD, NO_PARAM_TYPE, "ADD"};
ParamT ADD_paramt_template = {1, NULL};
Sentence_template ADD_template_sentence = {&ADD_token_template, &ADD_paramt_template};

Token INC_token_template = {ID, INC, NO_PARAM_TYPE, "INC"};
ParamT INC_paramt_template = {1, NULL};
Sentence_template INC_template_sentence = {&INC_token_template, &INC_paramt_template};

bool check_arithmetic_token_id(Token *token_id) {  // FIXME this fn is very manual
    const int tokens_id_size = sizeof(TOKENS_ID) / sizeof(TOKENS_ID[0]);

    for (unsigned int i = 0; i < tokens_id_size; i++) {
        bool matches = !strcmp(TOKENS_ID[i], token_id->content);
        if (!matches) continue;

        if (strcmp(TOKENS_ID[i], SUB_template_sentence.token->content) == 0) {
            token_id->type_name = SUB_template_sentence.token->type_name;
            return true;
        } else if (strcmp(TOKENS_ID[i], ADD_template_sentence.token->content) == 0) {
            token_id->type_name = ADD_template_sentence.token->type_name;
            return true;
        } else if (strcmp(TOKENS_ID[i], INC_template_sentence.token->content) == 0) {
            token_id->type_name = INC_template_sentence.token->type_name;
            return true;
        }
    }

    return false;
}

CheckParamsR check_arithmetic_params(TokenName token_id_name, Sentence st) {
    CheckParamsR cpr = {true, NULL};
    const Sentence_template sequence_templates[] = {ADD_template_sentence, SUB_template_sentence, INC_template_sentence};
    const int sequence_templates_count = sizeof(sequence_templates) / sizeof(sequence_templates[0]);

    for (unsigned int i = 0; i < sequence_templates_count; i++) {
        if (sequence_templates[i].token->type_name != token_id_name) continue;
        if (sequence_templates[i].paramT->count != st.tokens_count - 1) {
            cpr.success = false;
            const char *message = "Expected %d params but found %d for ID '%s'";
            const int expected_len = (int)log10(sequence_templates[i].paramT->count + 1) + 1;
            const int found_len = (int)log10(st.tokens_count + 1) + 1;

            cpr.message = malloc(sizeof(char) * (strlen(message) + expected_len + found_len + strlen(sequence_templates[i].token->content) - 2 * 3) + 1);
            sprintf(cpr.message, message, sequence_templates[i].paramT->count, st.tokens_count - 1, sequence_templates[i].token->content);
            return cpr;
        }
    }

    return cpr;
}
