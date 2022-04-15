//
//  mxdir.c
//  mpp-cpu
//
//  Created by https://github.com/nullxx on 23/3/22.
//

#include "addsub.h"

#include <stdio.h>

#include "../constants.h"
#include "../error.h"
#include "../pubsub.h"
#include "../thread.h"
#include "../utils.h"
#include "components.h"
#include "../electronic/bus.h"

LoadBit id_lb = {.value = 00};
Bus_t *last_bus_dir = NULL;

static PubSubSubscription *dir_bus_topic_subscription = NULL;

bool set_id_lb(unsigned long bin) {
    const int bin_len = get_used_bits(bin);
    if (bin_len > ID_LOAD_BIT_SIZE_BITS) {
        return false;
    }

    id_lb.value = bin;
    return true;
}

void run_addsub(void) {
    int dir_bus = word_to_int(last_bus_dir->next_value);

    switch (id_lb.value) {
        case 01:
            // +1
            dir_bus++;
            break;
        case 00:
            // +0
            break;
        case 11:
            // -1
            dir_bus--;
            break;
        default:
            break;
    }

    publish_message_to(DIR_BUS_TOPIC_1, int_to_word(dir_bus));
}

void init_addsub(void) {
    last_bus_dir = create_bus_data();
    dir_bus_topic_subscription = subscribe_to(DIR_BUS_TOPIC_2, last_bus_dir);
}

void shutdown_addsub(void) {
    unsubscribe_for(dir_bus_topic_subscription);
    destroy_bus_data(last_bus_dir);
}