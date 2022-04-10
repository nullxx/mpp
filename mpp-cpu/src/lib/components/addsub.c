//
//  mxdir.c
//  mpp-cpu
//
//  Created by https://github.com/nullxx on 23/3/22.
//

#include "addsub.h"

#include <stdio.h>

#include "../constants.h"
#include "../pubsub.h"
#include "../utils.h"
#include "components.h"

LoadBit id_lb = {.value = 00};
Bus_t last_bus_dir;
static PubSubSubscription *dir_bus_topic_subscription = NULL;

bool set_id_lb(unsigned long bin) {
    const int bin_len = get_bin_len(bin);
    if (bin_len > ID_LOAD_BIT_SIZE_BITS) {
        return false;
    }

    id_lb.value = bin;
    return true;
}

void run_addsub(void) {
    int dir_bus_int = bin_to_int(last_bus_dir);

    switch (id_lb.value) {
        case 01:
            // +1
            dir_bus_int++;
            break;
        case 00:
            // +0
            break;
        case 11:
            // -1
            dir_bus_int--;
            break;
        default:
            break;
    }

    unsigned long long next_bin_bus_dir = int_to_bin(dir_bus_int, MAX_CALC_BIN_SIZE_BITS);

    publish_message_to(DIR_BUS_TOPIC_1, next_bin_bus_dir);
}

void init_addsub(void) { dir_bus_topic_subscription = subscribe_to(DIR_BUS_TOPIC_2, &last_bus_dir); }
void shutdown_addsub(void) { unsubscribe_for(dir_bus_topic_subscription); }