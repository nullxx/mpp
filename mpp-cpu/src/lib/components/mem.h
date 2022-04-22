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
    int value;
    int offset;
    int initial;
} MemValue;

typedef struct {
    int values_count;
    MemValue *values;
} Mem;

void init_mem(void);
void shutdown_mem(void);
void run_mem(void);

#include "../error.h"
typedef struct {  // temporal
    int success;
    Error err;
    void *return_value;
} ComponentActionReturn;

MemValue *get_value_by_offset(int offset);
#endif /* mem_h */
