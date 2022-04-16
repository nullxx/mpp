/*
 * File: /src/lib/components/cu/d4flipflop.c
 * Project: mpp-cpu
 * File Created: Friday, 15th April 2022 3:41:39 pm
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Friday, 15th April 2022 3:42:29 pm
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */

#include "dxflipflop.h"

#include <stdio.h>

#include "../../pubsub.h"

Bus_t *d4flipflop_bus;
Bus_t *d3flipflop_bus;
Bus_t *d2flipflop_bus;
Bus_t *d1flipflop_bus;
Bus_t *d0flipflop_bus;
PubSubSubscription *d4_bus_topic_subscription = NULL;
PubSubSubscription *d3_bus_topic_subscription = NULL;
PubSubSubscription *d2_bus_topic_subscription = NULL;
PubSubSubscription *d1_bus_topic_subscription = NULL;
PubSubSubscription *d0_bus_topic_subscription = NULL;

void init_cu_dxflipflop(void) {
    d4flipflop_bus = create_bus_data();
    d3flipflop_bus = create_bus_data();
    d2flipflop_bus = create_bus_data();
    d1flipflop_bus = create_bus_data();
    d0flipflop_bus = create_bus_data();
    d4_bus_topic_subscription = subscribe_to(CU_SEQ_OUTPUT_D4_BUS_TOPIC, d4flipflop_bus);
    d3_bus_topic_subscription = subscribe_to(CU_SEQ_OUTPUT_D3_BUS_TOPIC, d3flipflop_bus);
    d2_bus_topic_subscription = subscribe_to(CU_SEQ_OUTPUT_D2_BUS_TOPIC, d2flipflop_bus);
    d1_bus_topic_subscription = subscribe_to(CU_SEQ_OUTPUT_D1_BUS_TOPIC, d1flipflop_bus);
    d0_bus_topic_subscription = subscribe_to(CU_SEQ_OUTPUT_D0_BUS_TOPIC, d0flipflop_bus);
}

void run_cu_dxflipflop(void) {
    publish_message_to(CU_SEQ_ACTUAL_STATUS_Q4_BUS_TOPIC, d4flipflop_bus->current_value);
    publish_message_to(CU_SEQ_ACTUAL_STATUS_Q3_BUS_TOPIC, d3flipflop_bus->current_value);
    publish_message_to(CU_SEQ_ACTUAL_STATUS_Q2_BUS_TOPIC, d2flipflop_bus->current_value);
    publish_message_to(CU_SEQ_ACTUAL_STATUS_Q1_BUS_TOPIC, d1flipflop_bus->current_value);
    publish_message_to(CU_SEQ_ACTUAL_STATUS_Q0_BUS_TOPIC, d0flipflop_bus->current_value);

    update_bus_data(d4flipflop_bus);
    update_bus_data(d3flipflop_bus);
    update_bus_data(d2flipflop_bus);
    update_bus_data(d1flipflop_bus);
    update_bus_data(d0flipflop_bus);
}

void shutdown_cu_dxflipflop(void) {
    unsubscribe_for(d4_bus_topic_subscription);
    unsubscribe_for(d3_bus_topic_subscription);
    unsubscribe_for(d2_bus_topic_subscription);
    unsubscribe_for(d1_bus_topic_subscription);
    unsubscribe_for(d0_bus_topic_subscription);

    destroy_bus_data(d4flipflop_bus);
    destroy_bus_data(d3flipflop_bus);
    destroy_bus_data(d2flipflop_bus);
    destroy_bus_data(d1flipflop_bus);
    destroy_bus_data(d0flipflop_bus);
}
