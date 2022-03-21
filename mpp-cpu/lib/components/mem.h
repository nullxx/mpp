//
//  mem.h
//  mpp-cpu
//
//  Created by Jon Lara trigo on 21/3/22.
//

#ifndef mem_h
#define mem_h

#include "components.h"

typedef struct {
	char *value_hex;
	char *offset;
} MemValue;

typedef struct {
	int values_count;
	MemValue *values;
} Mem;

void init_mem(on_component_change on_component_change_fn);

ComponentActionReturn get_mem_value(char *offset);
ComponentActionReturn set_mem_value(MemValue mem_value);

// flags
void set_l_e_flag(void);
void reset_l_e_flag(void);

#endif /* mem_h */
