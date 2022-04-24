/*
 * File: /src/lib/components/cu/mxreg_jnr.c
 * Project: mpp-cpu
 * File Created: Thursday, 1st January 1970 12:00:00 am
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Saturday, 16th April 2022 3:45:55 pm
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */
#include "mxreg_jnr.h"
#include <stdio.h>
#include "../../pubsub.h"

static Bus_t *last_bus_cu_ri_out = NULL;

static PubSubSubscription *last_cu_ri_output_subscription = NULL;

void init_cu_mxreg_jnr(void) {
    last_bus_cu_ri_out = create_bus_data();
    last_cu_ri_output_subscription = subscribe_to(CU_RI_OUTPUT_BUS_TOPIC, last_bus_cu_ri_out);
}

void run_cu_mxreg_jnr(void) {
    update_bus_data(last_bus_cu_ri_out);

    Word cop10, cop32;
    initialize_word(&cop10, 0);
    initialize_word(&cop32, 0);

    cop10.bits[0] = last_bus_cu_ri_out->current_value.bits[0];
    cop10.bits[1] = last_bus_cu_ri_out->current_value.bits[1];
    publish_message_to(CU_MXREG_JNR_OUTPUT_1_BUS_TOPIC, cop10);

    cop32.bits[0] = last_bus_cu_ri_out->current_value.bits[2];
    cop32.bits[1] = last_bus_cu_ri_out->current_value.bits[3];
    publish_message_to(CU_MXREG_JNR_OUTPUT_2_BUS_TOPIC, cop32);
}

void shutdown_cu_mxreg_jnr(void) {
    unsubscribe_for(last_cu_ri_output_subscription);

    destroy_bus_data(last_bus_cu_ri_out);
}
