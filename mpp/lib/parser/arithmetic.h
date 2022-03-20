//
//  arithmetic.h
//  mpp
//
//  Created by Jon Lara trigo on 20/3/22.
//

#ifndef arithmetic_h
#define arithmetic_h

#include "parser.h"

typedef struct {
    TokenName token_id_name;
    ParamType param_type;
} Param_template;

bool is_param_type_supported(TokenName token_id_name, Token t);

#endif /* arithmetic_h */
