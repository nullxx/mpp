/*
 * File: /src/lib/components/op2.c
 * Project: mpp-cpu
 * File Created: Wednesday, 30th March 2022 11:09:43 am
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Wednesday, 30th March 2022 11:10:07 am
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */

#include "op2.h"

#include <stdio.h>

#include "../constants.h"
#include "../pubsub.h"
#include "../watcher.h"
#include "components.h"
#include "../electronic/bus.h"

static PubSubSubscription *data_bus_topic_subscription = NULL;
static Bus_t *last_bus_data = NULL;

static Register op2_reg = {.bit_length = OP2_REG_SIZE_BIT};

static RegisterWatcher op2_reg_watcher = {.name = "2OP", .reg = &op2_reg};

static LoadBit car2_lb = {.value = 0};

void set_car2_lb(void) { car2_lb.value = 1; }
void reset_car2_lb(void) { car2_lb.value = 0; }

void init_op2(void) {
    initialize_word(&op2_reg.value, 0);

    last_bus_data = create_bus_data();
    data_bus_topic_subscription = subscribe_to(DATA_BUS_TOPIC, last_bus_data);
    register_watcher(&op2_reg_watcher);
}

void shutdown_op2(void) {
    unregister_watcher(&op2_reg_watcher);
    unsubscribe_for(data_bus_topic_subscription);
    destroy_bus_data(last_bus_data);
}

void run_op2(void) {
    update_bus_data(last_bus_data);

    if (car2_lb.value == 1) {
        // load
        op2_reg.value = last_bus_data->current_value;
    }

    publish_message_to(OP2_OUTPUT_BUS_TOPIC, op2_reg.value);
}