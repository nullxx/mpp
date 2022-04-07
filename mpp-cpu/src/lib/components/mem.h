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

void init_mem(void);
void shutdown_mem(void);
void run_mem(void); // <==

// control loadbits
void set_l_e_lb(void);
void reset_l_e_lb(void);
void set_mem_bus_lb(void);
void reset_mem_bus_lb(void);
// -- control loadbits

#include "../error.h"
typedef struct {  // temporal
    int success;
    Error err;
    void *return_value;
} ComponentActionReturn;

ComponentActionReturn set_mem_value(MemValue mem_value);
#endif /* mem_h */
