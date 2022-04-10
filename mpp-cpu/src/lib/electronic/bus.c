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

#include "../constants.h"
#include "../pubsub.h"
#include "../utils.h"

static int check_data_bus_overflow(Bus_t value) {
    if (get_bin_len(value) > DATA_BUS_SIZE_BITS) {
        return 0;
    }

    return 1;
}

static int check_dir_bus_overflow(Bus_t value) {
    if (get_bin_len(value) > DIR_BUS_SIZE_BITS) {
        return 0;
    }

    return 1;
}

static void check_buses_overflow(void) {
    // data bus
    add_topic_middleware(DATA_BUS_TOPIC, check_data_bus_overflow);

    // dir bus
    add_topic_middleware(DIR_BUS_TOPIC_1, check_dir_bus_overflow);
    add_topic_middleware(DIR_BUS_TOPIC_2, check_dir_bus_overflow);
}

void init_buses(void) {
    check_buses_overflow();
}