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


static const int mem_size_bits = MEM_SIZE_KB * 1024 * 8;
static const int mem_size = mem_size_bits / MEM_VALUE_SIZE_BITS;

static Mem mem;

// READ by default
static LoadBit l_e_lb = {.value = 0};
static LoadBit mem_bus_lb = {.value = 0};

static Bus_t last_bus_data;
static Bus_t last_bus_dir;

static PubSubSubscription *bus_data_subscription = NULL;
static PubSubSubscription *bus_dir_subscription = NULL;

static int is_mem_value_valid(char *hex) {
    const int num = hex_to_int(hex);
    if (num < 0 || num > pow(2, MEM_VALUE_SIZE_BITS) - 1) return 0;
    return 1;
}

static void fill_memory(void) {
    mem.values = (MemValue *)malloc(sizeof(MemValue) * mem_size);
    mem.values_count = 0;

    for (int i = MEM_START_VALUE; i < mem_size; i++) {
        MemValue mem_value;
        mem_value.offset = int_to_hex(i);
        mem_value.value_hex = int_to_hex(random_int(0, pow(2, MEM_VALUE_SIZE_BITS) - 1));
        mem.values[i] = mem_value;
        mem.values_count++;
    }
}

static void unfill_memory(void) {
    for (int i = MEM_START_VALUE; i < mem_size; i++) {
        free(mem.values[i].offset);
        free(mem.values[i].value_hex);
    }
    free(mem.values);
}

void init_mem(void) {
    fill_memory();
    bus_data_subscription = subscribe_to(DATA_BUS_TOPIC, &last_bus_data);
    bus_dir_subscription = subscribe_to(DIR_BUS_TOPIC_2, &last_bus_dir);
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

static MemValue *get_value_by_offset(char *offset) {
    for (int i = 0; i < mem.values_count; i++) {
        if (!strcmp(mem.values[i].offset, offset)) {
            return &mem.values[i];
        }
    }

    return NULL;
}

// -- INTERNAL
// TODO add static again after debugging, and remove header
ComponentActionReturn set_mem_value(MemValue mem_value) {
    ComponentActionReturn car;
    car.success = 1;

    if (!is_mem_value_valid(mem_value.value_hex)) {
        car.success = 0;
        car.err.show_errno = 0;
        car.err.type = FATAL_ERROR;
        const int end_bound_mem_value = pow(2, MEM_VALUE_SIZE_BITS) - 1;
        car.err.message = create_str("[set_mem_value] Value", mem_value.value_hex, "invalid. Must be between 0 -", itoa(end_bound_mem_value));
        return car;
    }

    MemValue *target_mem_value = get_value_by_offset(mem_value.offset);
    if (target_mem_value == NULL) {
        car.success = 0;
        car.err.show_errno = 0;
        car.err.type = FATAL_ERROR;
        car.err.message = create_str("[set_mem_value] Couldn't find mem_value at", mem_value.offset);
        return car;
    }

    free(target_mem_value->offset);
    free(target_mem_value->value_hex);

    target_mem_value->value_hex = str_dup(mem_value.value_hex);
    target_mem_value->offset = str_dup(mem_value.offset);

    return car;
}

static ComponentActionReturn get_mem_value(char *offset) {
    ComponentActionReturn car;
    car.success = 1;

    MemValue *target_mem_value = get_value_by_offset(offset);
    if (target_mem_value == NULL) {
        car.success = 0;
        car.err.show_errno = 0;
        car.err.type = FATAL_ERROR;
        car.err.message = create_str("[get_mem_value] Couldn't find mem_value at", offset);
        return car;
    }

    car.return_value = (void *)target_mem_value;

    return car;
}

void run_mem(void) {
    Error err;
    char *dir_bin_str = int_to_hex(bin_to_int(last_bus_dir));
    char *value_bin_str = int_to_hex(bin_to_int(last_bus_data));

    switch (l_e_lb.value) {
        case 1: {
            ComponentActionReturn car = get_mem_value(dir_bin_str);
            if (!car.success) {
                err = car.err;
                goto error;
            }

            MemValue *m = (MemValue *)car.return_value;

            // if memBus ==> send data to the bus
            unsigned long long bin = int_to_bin(hex_to_int(m->value_hex), MAX_CALC_BIN_SIZE_BITS);
            if (mem_bus_lb.value == 1) publish_message_to(DATA_BUS_TOPIC, bin);
            break;
        }
        case 0: {
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
    return throw_error(err);
}
