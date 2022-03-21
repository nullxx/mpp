//
//  arithmetic.c
//  mpp
//
//  Created by Jon Lara trigo on 20/3/22.
//

#include "arithmetic.h"

#include <stdio.h>

Param_template ADD_template_param = {ADD, INM_OR_REG_OR_AC};
Param_template SUB_template_param = {SUB, INM_OR_REG_OR_AC};
Param_template INC_template_param = {INC, REG_OR_AC};

bool is_param_type_supported(TokenName token_id_name, Token t) {
	const Param_template param_templates[] = {ADD_template_param, SUB_template_param, INC_template_param};
	const size_t param_templates_count = sizeof(param_templates) / sizeof(param_templates[0]);

	for (unsigned int i = 0; i < param_templates_count; i++) {
		if (token_id_name != param_templates[i].token_id_name) continue;
		if (param_templates[i].param_type == INM_OR_REG_OR_AC && (t.param_type == INM || t.param_type == REG || t.param_type == AC)) return true;
		if (param_templates[i].param_type == REG_OR_AC && (t.param_type == REG || t.param_type == AC)) return true;

		if (param_templates[i].param_type == t.param_type) return true;
	}

	return false;
}
