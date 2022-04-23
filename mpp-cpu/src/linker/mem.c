/*
 * File: /src/linker/mem.c
 * Project: mpp-cpu
 * File Created: Friday, 22nd April 2022 2:44:53 am
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Friday, 22nd April 2022 2:45:16 am
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */

#include <stdio.h>

#include "../lib/constants.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "../lib/components/mem.h"

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
int get_memory_size(void) {
    const int mem_size_bits = MEM_SIZE_KB * 1024 * 8;
    const int mem_size = mem_size_bits / MEM_VALUE_SIZE_BITS;
    return mem_size;
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
int get_memory_value(const int offset) {
    MemValue *mem = get_value_by_offset(offset);
    if (mem == NULL) {
        return -1;
    }
    return mem->value;
}