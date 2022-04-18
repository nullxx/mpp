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
#include "../electronic/bus.h"
#include "../error.h"
#include "../pubsub.h"
#include "../thread.h"
#include "../utils.h"

// INTERNAL

static const int mem_size_bits = MEM_SIZE_KB * 1024 * 8;
static const int mem_size = mem_size_bits / MEM_VALUE_SIZE_BITS;

static Mem mem;

static Bus_t *last_bus_data = NULL;
static Bus_t *last_bus_dir = NULL;
static Bus_t *control_bus = NULL;

static PubSubSubscription *bus_data_subscription = NULL;
static PubSubSubscription *bus_dir_subscription = NULL;
static PubSubSubscription *control_bus_topic_subscription = NULL;

static int is_mem_value_valid(int num) {
    if (num < 0 || num > pow(2, MEM_VALUE_SIZE_BITS) - 1) return 0;
    return 1;
}

static void fill_memory(void) {
    mem.values = (MemValue *)malloc(sizeof(MemValue) * mem_size);
    mem.values_count = 0;

    for (int i = MEM_START_VALUE; i < mem_size; i++) {
        MemValue mem_value;
        mem_value.offset = i;
        mem_value.value = random_int(0, pow(2, MEM_VALUE_SIZE_BITS) - 1);
        mem_value.initial = 1;
        mem.values[i] = mem_value;
        mem.values_count++;
    }
}

static void unfill_memory(void) { free(mem.values); }

void init_mem(void) {
    fill_memory();
    last_bus_data = create_bus_data();
    last_bus_dir = create_bus_data();
    control_bus = create_bus_data();
    bus_data_subscription = subscribe_to(DATA_BUS_TOPIC, last_bus_data);
    bus_dir_subscription = subscribe_to(DIR_BUS_TOPIC_2, last_bus_dir);
    control_bus_topic_subscription = subscribe_to(CONTROL_BUS_TOPIC, control_bus);
}

void shutdown_mem(void) {
    unfill_memory();

    unsubscribe_for(bus_data_subscription);
    unsubscribe_for(bus_dir_subscription);
    unsubscribe_for(control_bus_topic_subscription);

    destroy_bus_data(last_bus_data);
    destroy_bus_data(last_bus_dir);
    destroy_bus_data(control_bus);
}

static MemValue *get_value_by_offset(int offset) {
    if (offset < MEM_START_VALUE || offset >= mem_size) {
        return NULL;
    }

    return &mem.values[offset];
}

// -- INTERNAL
// TODO add static again after debugging, and remove header
ComponentActionReturn set_mem_value(MemValue mem_value) {
    ComponentActionReturn car;
    car.success = 1;

    if (!is_mem_value_valid(mem_value.value)) {
        car.success = 0;
        car.err.show_errno = 0;
        car.err.type = NOTICE_ERROR;
        const int end_bound_mem_value = pow(2, MEM_VALUE_SIZE_BITS) - 1;
        char *value_dec_str = num_to_str(mem_value.value);
        car.err.message = create_str("[set_mem_value] Value", value_dec_str, "(dec) invalid. Must be between 0 -", num_to_str(end_bound_mem_value));
        return car;
    }

    MemValue *target_mem_value = get_value_by_offset(mem_value.offset);
    if (target_mem_value == NULL) {
        car.success = 0;
        car.err.show_errno = 0;
        car.err.type = NOTICE_ERROR;
        car.err.message = create_str("[set_mem_value] Couldn't find mem_value at", mem_value.offset);
        return car;
    }

    target_mem_value->value = mem_value.value;
    target_mem_value->offset = mem_value.offset;

    return car;
}

static ComponentActionReturn get_mem_value(int offset) {
    ComponentActionReturn car;
    car.success = 1;

    MemValue *target_mem_value = get_value_by_offset(offset);
    if (target_mem_value == NULL) {
        car.success = 0;
        car.err.show_errno = 0;
        car.err.type = NOTICE_ERROR;
        car.err.message = create_str("[get_mem_value] Couldn't find mem_value at", offset);
        return car;
    }

    car.return_value = (void *)target_mem_value;

    return car;
}

void run_mem(void) {
    update_bus_data(last_bus_data);
    update_bus_data(last_bus_dir);
    update_bus_data(control_bus);

    Error err;
    int dir_bin = word_to_int(last_bus_dir->next_value);

    Word l_e_lb;
    Word mem_bus_lb;
    initialize_word(&l_e_lb, 0);
    initialize_word(&mem_bus_lb, 0);

    l_e_lb.bits[0] = control_bus->current_value.bits[CONTROL_BUS_LE_BIT_POSITION];

    mem_bus_lb.bits[0] = control_bus->current_value.bits[CONTROL_BUS_MEMBUS_BIT_POSITION];

    switch (word_to_int(l_e_lb)) {
        case 1: {
            ComponentActionReturn car = get_mem_value(dir_bin);
            if (!car.success) {
                err = car.err;
                goto error;
            }

            MemValue *m = (MemValue *)car.return_value;

            // if memBus ==> send data to the bus
            if (word_to_int(mem_bus_lb) == 1) publish_message_to(DATA_BUS_TOPIC, int_to_word(m->value));
            break;
        }
        case 0: {
            // use the last value of DATA_BUS
            MemValue mem_value = {.offset = dir_bin, .value = word_to_int(last_bus_data->next_value)};
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

    return;
error:
    return throw_error(err);
}

// for the simulation
Mem get_mem(void) { return mem; }