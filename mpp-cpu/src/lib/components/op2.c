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
#include "../electronic/bus.h"
#include "../pubsub.h"
#include "../watcher.h"
#include "components.h"

static Bus_t *last_bus_data = NULL;
static Bus_t *control_bus = NULL;

static PubSubSubscription *data_bus_topic_subscription = NULL;
static PubSubSubscription *control_bus_topic_subscription = NULL;

static Register op2_reg = {.bit_length = OP2_REG_SIZE_BIT};

static RegisterWatcher op2_reg_watcher = {.reg = &op2_reg, .type = WATCHER_TYPE_2OP};

void init_op2(void) {
    initialize_word(&op2_reg.value, 0);

    last_bus_data = create_bus_data();
    control_bus = create_bus_data();
    data_bus_topic_subscription = subscribe_to(DATA_BUS_TOPIC, last_bus_data);
    control_bus_topic_subscription = subscribe_to(CONTROL_BUS_TOPIC, control_bus);

    register_watcher(&op2_reg_watcher);
}

void shutdown_op2(void) {
    unregister_watcher(&op2_reg_watcher);
    unsubscribe_for(data_bus_topic_subscription);
    unsubscribe_for(control_bus_topic_subscription);

    destroy_bus_data(last_bus_data);
    destroy_bus_data(control_bus);
}

void run_op2(void) {
    update_bus_data(last_bus_data);
    update_bus_data(control_bus);

    Word car2_lb;
    initialize_word(&car2_lb, 0);

    car2_lb.bits[0] = control_bus->current_value.bits[CONTROL_BUS_OP2CAR_BIT_POSITION];

    if (word_to_int(car2_lb) == 1) {
        // load
        op2_reg.value = last_bus_data->current_value;
    }

    publish_message_to(OP2_OUTPUT_BUS_TOPIC, op2_reg.value);
}