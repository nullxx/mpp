//
//  parser.c
//  mpp
//
//  Created by Jon Lara trigo on 20/3/22.
//

#include "parser.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "../error.h"
#include "../logger.h"
#include "arithmetic.h"

const unsigned char INM_MAX_HEX = 0xFF;
const char MIN_REG = 'A';
const char MAX_REG = 'E';
const char *AC_CONTENT = "AC";

bool validate_ac(char *str) {
	if (!strcmp(str, AC_CONTENT)) {
		return true;
	}

	return false;
}

bool validate_param_inm(char *hex_str) {
	const int number = (int)strtol(hex_str, NULL, 16);
	if (number == 0 || number > INM_MAX_HEX) return false;
	return true;
}

bool validate_param_reg(char *reg_str) {
	const size_t reg_str_len = strlen(reg_str);
	if (reg_str_len != 1) return false;

	char reg = (char)reg_str[0];
	if (reg < MIN_REG || reg > MAX_REG) return false;
	return true;
}

ParamType get_param_type(Token t) {
	if (t.type != PARAM) return NO_PARAM_TYPE;
	bool is_param_ac = validate_ac(t.content);
	bool is_param_reg = validate_param_reg(t.content);
	bool is_param_inm = validate_param_inm(t.content);

	if (is_param_ac) return AC;
	if (is_param_reg) return REG;
	if (is_param_inm) return INM;

	return NO_PARAM_TYPE;
}
// recursive descent parse
void parse_sentence(Sentence *st) {
	Error err;
	TokenName token_id_name = st->tokens[0].type_name;
	for (unsigned int i = 1; i < st->tokens_count; i++) { // from 1 to ignore ID
		ParamType p_type = get_param_type(st->tokens[i]);
		st->tokens[i].param_type = p_type;

		bool param_supported = is_param_type_supported(token_id_name, st->tokens[i]);
		if (!param_supported) {
			err.type = FATAL;
			const char *message = "Param '%s' is not supported at line %d";
			const int row_num_len = (int)log10((st->row + 1) + 1) + 1;

			err.message = malloc(sizeof(char) * (strlen(message) + strlen(st->tokens[i].content) + row_num_len - 2 * 2) + 1);
			sprintf(err.message, message, st->tokens[i].content, st->row + 1);
			goto exception;
		}
	}

exception:
	process_error(&err);
}

/**
 *  Runs the parser. Thows a FATAL error if any param is not supported. Modifies sentencest tokens setting param_type attribute.
 */
void run_parser(SentenceT *sentencest) {
	for (unsigned int i = 0; i < sentencest->count; i++) {
		parse_sentence(&sentencest->sencences[i]);
	}
}
