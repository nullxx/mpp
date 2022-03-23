//
//  mem.c
//  mpp-cpu
//
//  Created by Jon Lara trigo on 21/3/22.
//

#include "mem.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../constants.h"
#include "../error.h"
#include "../pubsub.h"
#include "../utils.h"

// INTERNAL
const int mem_size_bits = MEM_SIZE_KB * 1024 * 8;
const int mem_size = mem_size_bits / MEM_VALUE_SIZE_BITS;

Mem mem;

// READ by default
LoadBit l_e_lb = {.value = 0};
LoadBit mem_bus_lb = {.value = 0};

DataBus_t last_bus_data;
DirBus_t last_bus_dir;

PubSubSubscription *bus_data_subscription = NULL;
PubSubSubscription *bus_dir_subscription = NULL;

bool is_mem_value_valid(char *hex) {
    const int num = hex_to_int(hex);
    if (num < 0 || num > pow(2, MEM_VALUE_SIZE_BITS) - 1) return false;
    return true;
}

void fill_memory(void) {
    mem.values = (MemValue *)malloc(sizeof(MemValue) * mem_size);
    mem.values_count = 0;

    for (int i = MEM_START_VALUE; i < mem_size; i++) {
        MemValue mem_value;
        mem_value.offset = int_to_hex(i);
        mem_value.value_hex = int_to_hex(random_int(pow(2, MEM_VALUE_SIZE_BITS) - 1));
        mem.values[i] = mem_value;
        mem.values_count++;
    }
}

void unfill_memory(void) {
    for (int i = MEM_START_VALUE; i < mem_size; i++) {
        free(mem.values[i].offset);
        free(mem.values[i].value_hex);
    }
    free(mem.values);
}

void on_bus_data_message(PubSubMessage m) {
    // no condition to receive data from data bus --> always receiving data
    last_bus_data = (DataBus_t)m.value;
}

void on_bus_dir_message(PubSubMessage m) {
    // no condition to receive data from dir bus --> always receiving data
    last_bus_dir = (DirBus_t)m.value;
}

void init_mem(void) {
    fill_memory();
    bus_data_subscription = subscribe_to(DATA_BUS_TOPIC, on_bus_data_message);
    bus_dir_subscription = subscribe_to(DIR_BUS_TOPIC, on_bus_dir_message);
}

void shutdown_mem(void) {
    unfill_memory();
    unsubscribe_for(bus_data_subscription);
    unsubscribe_for(bus_dir_subscription);
}

// control loadbits functions
void set_l_e_lb(void) { l_e_lb.value = 1; }

void reset_l_e_lb(void) { l_e_lb.value = 0; }

void set_mem_bus_lb(void) { mem_bus_lb.value = 1; }

void reset_mem_bus_lb(void) { mem_bus_lb.value = 0; }
// -- control loadbits functions

MemValue *get_value_by_offset(char *offset) {
    for (int i = 0; i < mem.values_count; i++) {
        if (!strcmp(mem.values[i].offset, offset)) {
            return &mem.values[i];
        }
    }

    return NULL;
}

// -- INTERNAL

ComponentActionReturn set_mem_value(MemValue mem_value) {
    ComponentActionReturn car;
    car.success = false;

    if (!is_mem_value_valid(mem_value.value_hex)) {
        car.success = false;
        car.err.show_errno = false;
        const char *msg = "[set_mem_value] Value %s invalid. Must be between 0-%d";
        const int end_bound_mem_value = pow(2, MEM_VALUE_SIZE_BITS) - 1;
        const int end_bound_mem_value_size = (int)log10((end_bound_mem_value + 1) + 1) + 1;
        const size_t size = (size_t)sizeof(char) * (strlen(msg) + strlen(mem_value.value_hex) + end_bound_mem_value_size - 2 * 2 + 1);
        char *message = (char *)malloc(size);
        snprintf(message, size, msg, mem_value.value_hex, end_bound_mem_value);
        car.err.message = message;
        return car;
    }

    MemValue *target_mem_value = get_value_by_offset(mem_value.offset);
    if (target_mem_value == NULL) {
        car.success = false;
        car.err.show_errno = false;
        const char *msg = "[set_mem_value] Couldn't find mem_value at %0x";
        const size_t size = (size_t)sizeof(char) * (strlen(msg) + strlen(mem_value.offset) - 3 + 1);
        char *message = (char *)malloc(size);
        snprintf(message, size, msg, mem_value.offset);
        car.err.message = message;
        return car;
    }

    free(target_mem_value->offset);
    free(target_mem_value->value_hex);

    target_mem_value->value_hex = mem_value.value_hex;
    target_mem_value->offset = mem_value.offset;

    return car;
}

ComponentActionReturn get_mem_value(char *offset) {
    ComponentActionReturn car;
    car.success = false;

    MemValue *target_mem_value = get_value_by_offset(offset);
    if (target_mem_value == NULL) {
        car.success = false;
        car.err.show_errno = false;
        const char *msg = "[get_mem_value] Couldn't find mem_value at %0x";
        const size_t size = (size_t)sizeof(char) * (strlen(msg) + strlen(offset) - 3 + 1);
        char *message = (char *)malloc(size);
        snprintf(message, size, msg, offset);
        car.err.message = message;
        return car;
    }

    car.return_value = (void *)target_mem_value;

    return car;
}

void run_mem(void) {
    Error err;
    char *dir_bin_str = bin_to_str(last_bus_dir);
    char *value_bin_str = bin_to_str(last_bus_data);

    switch (l_e_lb.value) {
        case 0: {
            ComponentActionReturn car = get_mem_value(dir_bin_str);
            if (!car.success) {
                err = car.err;
                goto error;
            }

            MemValue *m = (MemValue *)car.return_value;

            // if memBus ==> send data to the bus
            if (mem_bus_lb.value == 1) publish_message_to(DATA_BUS_TOPIC, (void *)int_to_bin(hex_to_int(m->value_hex)));
            break;
        }
        case 1: {
            // use the last value of DATA_BUS
            MemValue mem_value = {.offset = dir_bin_str, .value_hex = value_bin_str};
            ComponentActionReturn car = set_mem_value(mem_value);
            if (!car.success) {
                err = car.err;
                goto error;
            }

            break;
        }
        default:
            break;
    }

    free(dir_bin_str);
    free(value_bin_str);

    return;
error:
    process_error(&err);
}
