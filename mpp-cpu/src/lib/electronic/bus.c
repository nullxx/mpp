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

#include <stdbool.h>

#include "../constants.h"
#include "../pubsub.h"
#include "../utils.h"

static bool check_data_bus_overflow(void *value) {
    DataBus_t data_bus_bin = *(DataBus_t*)value;
    if (get_bin_len(data_bus_bin) > DATA_BUS_SIZE_BITS) {
        return false;
    }

    return true;
}

static bool check_dir_bus_overflow(void *value) {
    DirBus_t dir_bus_bin = *(DirBus_t*)value;
    if (get_bin_len(dir_bus_bin) > DIR_BUS_SIZE_BITS) {
        return false;
    }

    return true;
}

static void check_buses_overflow(void) {
    // data bus
    add_topic_middleware(DATA_BUS_TOPIC, check_data_bus_overflow);

    // dir bus
    add_topic_middleware(DIR_BUS_TOPIC, check_dir_bus_overflow);
}

void init_buses(void) {
    check_buses_overflow();
}