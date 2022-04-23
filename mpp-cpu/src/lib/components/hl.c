/*
 * File: /src/lib/components/hl.c
 * Project: mpp-cpu
 * File Created: Sunday, 27th March 2022 10:53:23 pm
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Sunday, 27th March 2022 10:54:08 pm
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */

#include "hl.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../constants.h"
#include "../electronic/bus.h"
#include "../error.h"
#include "../pubsub.h"
#include "../utils.h"
#include "../watcher.h"
#include "components.h"

static Register h_reg = {.bit_length = H_REG_SIZE_BIT};
static Register l_reg = {.bit_length = L_REG_SIZE_BIT};

static RegisterWatcher h_reg_watcher = {.reg = &h_reg, .type = WATCHER_TYPE_H};
static RegisterWatcher l_reg_watcher = {.reg = &l_reg, .type = WATCHER_TYPE_L};

static Bus_t *last_bus_data = NULL;
static Bus_t *control_bus = NULL;

static PubSubSubscription *data_bus_topic_subscription = NULL;
static PubSubSubscription *control_bus_topic_subscription = NULL;

void init_hl(void) {
    initialize_word(&h_reg.value, 0);
    initialize_word(&l_reg.value, 0);

    last_bus_data = create_bus_data();
    control_bus = create_bus_data();
    data_bus_topic_subscription = subscribe_to(DATA_BUS_TOPIC, last_bus_data);
    control_bus_topic_subscription = subscribe_to(CONTROL_BUS_TOPIC, control_bus);

    register_watcher(&h_reg_watcher);
    register_watcher(&l_reg_watcher);
}

void shutdown_hl(void) {
    unregister_watcher(&h_reg_watcher);
    unregister_watcher(&l_reg_watcher);

    unsubscribe_for(data_bus_topic_subscription);
    unsubscribe_for(control_bus_topic_subscription);

    destroy_bus_data(last_bus_data);
    destroy_bus_data(control_bus);
}

void run_hl(void) {
    update_bus_data(last_bus_data);
    update_bus_data(control_bus);

    Word hcar_lb;
    Word lcar_lb;
    initialize_word(&hcar_lb, 0);
    initialize_word(&lcar_lb, 0);

    hcar_lb.bits[0] = control_bus->current_value.bits[CONTROL_BUS_HCAR_BIT_POSITION];
    
    lcar_lb.bits[0] = control_bus->current_value.bits[CONTROL_BUS_LCAR_BIT_POSITION];

    if (word_to_int(hcar_lb) == 1) {
        // load h
        h_reg.value = last_bus_data->current_value;
    }

    if (word_to_int(lcar_lb) == 1) {
        // load l
        l_reg.value = last_bus_data->current_value;
    }

    Word to_send;
    initialize_word(&to_send, 0);

    // H|L
    for (int i = 0; i < l_reg.bit_length; i++) {
        to_send.bits[i] = l_reg.value.bits[i];
    }

    for (int i = 0; i < h_reg.bit_length; i++) {
        to_send.bits[l_reg.bit_length + i] = h_reg.value.bits[i];
    }

    publish_message_to(HL_OUTPUT_BUS_TOPIC, to_send);
}
