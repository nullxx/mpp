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
#include "components.h"

static PubSubSubscription *data_bus_topic_subscription = NULL;
static DataBus_t last_bus_data;

static Register op2_reg = {.bin_value = 0, .bit_length = OP2_REG_SIZE_BIT};

static LoadBit car2_lb = {.value = 0};

static void on_bus_data_message(PubSubMessage m) { last_bus_data = (DataBus_t)m.value; }

void set_car2_lb(void) { car2_lb.value = 1; }
void reset_car2_lb(void) { car2_lb.value = 0; }

void init_op2(void) { data_bus_topic_subscription = subscribe_to(DATA_BUS_TOPIC, on_bus_data_message); }
void shutdown_op2(void) { unsubscribe_for(data_bus_topic_subscription); }

void run_op2(void) {
    if (car2_lb.value == 1) {
        // load
        op2_reg.bin_value = last_bus_data;
    }

    publish_message_to(OP2_OUTPUT_BUS_TOPIC, &op2_reg.bin_value);
}