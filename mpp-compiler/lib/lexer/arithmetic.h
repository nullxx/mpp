//
//  aritmetic.h
//  mpp
//
//  Created by Jon Lara trigo on 19/3/22.
//

#ifndef arithmetic_h
#define arithmetic_h

#include <stdbool.h>

#include "../file.h"
#include "lexer.h"

bool check_arithmetic_token_id(Token *token_id);
CheckParamsR check_arithmetic_params(TokenName token_id_name, Sentence st);

#endif /* arithmetic_h */
