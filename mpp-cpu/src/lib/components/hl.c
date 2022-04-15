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

static RegisterWatcher h_reg_watcher = {.name = "H", .reg = &h_reg};
static RegisterWatcher l_reg_watcher = {.name = "L", .reg = &l_reg};

static LoadBit hcar_lb = {.value = 0};

static LoadBit lcar_lb = {.value = 0};
static PubSubSubscription *data_bus_topic_subscription = NULL;
static Bus_t *last_bus_data = NULL;

void set_hcar_lb(void) { hcar_lb.value = 1; }
void reset_hcar_lb(void) { hcar_lb.value = 0; }

void set_lcar_lb(void) { lcar_lb.value = 1; }
void reset_lcar_lb(void) { lcar_lb.value = 0; }

void init_hl(void) {
    initialize_word(&h_reg.value, 0);
    initialize_word(&l_reg.value, 0);

    last_bus_data = create_bus_data();
    data_bus_topic_subscription = subscribe_to(DATA_BUS_TOPIC, last_bus_data);

    register_watcher(&h_reg_watcher);
    register_watcher(&l_reg_watcher);
}

void shutdown_hl(void) {
    unregister_watcher(&h_reg_watcher);
    unregister_watcher(&l_reg_watcher);

    unsubscribe_for(data_bus_topic_subscription);
    destroy_bus_data(last_bus_data);
}

void run_hl(void) {
    update_bus_data(last_bus_data);

    if (hcar_lb.value == 1) {
        // load h
        h_reg.value = last_bus_data->current_value;
    }

    if (lcar_lb.value == 1) {
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
