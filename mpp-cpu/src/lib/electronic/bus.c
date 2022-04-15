/*
 * File: /src/lib/electronic/bus.c
 * Project: mpp-cpu
 * File Created: Thursday, 24th March 2022 8:06:53 pm
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Thursday, 24th March 2022 8:06:55 pm
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */

#include "bus.h"

#include <stdlib.h>

#include "../constants.h"
#include "../definitions.h"
#include "../pubsub.h"
#include "../utils.h"

static int check_data_bus_overflow(Word value) {
    if (get_used_bits(word_to_int(value)) > DATA_BUS_SIZE_BITS) {
        return 0;
    }

    return 1;
}

static int check_dir_bus_overflow(Word value) {
    if (get_used_bits(word_to_int(value)) > DIR_BUS_SIZE_BITS) {
        return 0;
    }

    return 1;
}

Bus_t *create_bus_data(void) {
    Bus_t *bus_t = (Bus_t *)malloc(sizeof(Bus_t));
    initialize_word(&bus_t->next_value, 0);
    initialize_word(&bus_t->current_value, 0);

    return bus_t;
}

void create_bus_data_from(Bus_t *bus_t) {
    initialize_word(&bus_t->next_value, 0);
    initialize_word(&bus_t->current_value, 0);
}

void destroy_bus_data(Bus_t *bus_t) { free(bus_t); }

void update_bus_data(Bus_t *bus_t) { bus_t->current_value = bus_t->next_value; }

static void check_buses_overflow(void) {
    // data bus
    add_topic_middleware(DATA_BUS_TOPIC, check_data_bus_overflow);

    // dir bus
    add_topic_middleware(DIR_BUS_TOPIC_1, check_dir_bus_overflow);
    add_topic_middleware(DIR_BUS_TOPIC_2, check_dir_bus_overflow);
}

void init_buses(void) { check_buses_overflow(); }