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
#include "../lib/pubsub.h"
#include "linker.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "../lib/components/mem.h"

static Bus_t *last_bus_dir = NULL;
static PubSubSubscription *bus_dir_subscription = NULL;

void init_linker_mem(void) {
    last_bus_dir = create_bus_data();
    bus_dir_subscription = subscribe_to(DIR_BUS_TOPIC_2, last_bus_dir);
}

void shutdown_linker_mem(void) {
    unsubscribe_for(bus_dir_subscription);
    destroy_bus_data(last_bus_dir);
}

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
int get_memory_value_size_bits(void) { return MEM_VALUE_SIZE_BITS; }

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

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void set_memory_value(const int offset, const int value) {
    get_value_by_offset(offset)->value = value;
    get_update_ui_fn()();
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
int get_memory_dir_bus(void) { return word_to_int(last_bus_dir->next_value); }