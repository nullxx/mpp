/*
 * File: /src/lib/components/cu/seq_b_jnr.c
 * Project: mpp-cpu
 * File Created: Thursday, 14th April 2022 4:54:54 pm
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Thursday, 14th April 2022 4:55:01 pm
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */

#define RI_START_BIT_POS 2
#include "seq_fgs_jnr.h"

#include <stdio.h>

#include "../../electronic/bus.h"
#include "../../pubsub.h"
#include "../../utils.h"

static Bus_t *last_bus_cu_ri_out = NULL;
static Bus_t *last_bus_flags_out = NULL;

PubSubSubscription *last_cu_ri_output_subscription = NULL;
PubSubSubscription *flags_out_bus_topic_subscription = NULL;

void init_cu_seq_b_jnr(void) {
    last_bus_cu_ri_out = create_bus_data();
    last_bus_flags_out = create_bus_data();
    last_cu_ri_output_subscription = subscribe_to(CU_RI_OUTPUT_BUS_TOPIC, last_bus_cu_ri_out);
    flags_out_bus_topic_subscription = subscribe_to(FLAGS_OUTPUT_BUS_TOPIC, last_bus_flags_out);
}

void run_cu_seq_b_jnr(void) {
    update_bus_data(last_bus_cu_ri_out);
    update_bus_data(last_bus_flags_out);

    Word to_send;
    initialize_word(&to_send, 0);

    to_send.bits[0] = last_bus_flags_out->current_value.bits[1];  // fc
    to_send.bits[1] = last_bus_flags_out->current_value.bits[0];  // fz

    for (int i = 0; i < WORD_SIZE_BIT - RI_START_BIT_POS; i++) {
        to_send.bits[i + RI_START_BIT_POS] = last_bus_cu_ri_out->current_value.bits[i];
    }

    publish_message_to(CU_SEQ_JOINER_OUTPUT_BUS_TOPIC, to_send);
}

void shutdown_cu_seq_b_jnr(void) {
    unsubscribe_for(last_cu_ri_output_subscription);
    unsubscribe_for(flags_out_bus_topic_subscription);
    destroy_bus_data(last_bus_cu_ri_out);
    destroy_bus_data(last_bus_flags_out);
}