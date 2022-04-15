/*
 * File: /src/lib/components/cu/seq_b_jnr.c
 * Project: mpp-cpu
 * File Created: Thursday, 14th April 2022 4:54:54 pm
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Thursday, 14th April 2022 4:55:01 pm
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */

#include "seq_fgs_jnr.h"

#include "../../electronic/bus.h"
#include "../../pubsub.h"
#include "../../utils.h"
Bus_t last_bus_cu_ri_out = {.current_value = 0, .next_value = 0};
Bus_t last_bus_flags_out = {.current_value = 0, .next_value = 0};

PubSubSubscription *last_cu_ri_output_subscription;
PubSubSubscription *flags_out_bus_topic_subscription;

void init_cu_seq_b_jnr(void) {
    last_cu_ri_output_subscription = subscribe_to(PC_OUTPUT_BUS_TOPIC, &last_bus_cu_ri_out);
    flags_out_bus_topic_subscription = subscribe_to(PC_OUTPUT_BUS_TOPIC, &last_bus_flags_out);
}

void run_cu_seq_b_jnr(void) {
    update_bus_data(&last_bus_cu_ri_out);
    update_bus_data(&last_bus_flags_out);

    Bin seq_output_bin = last_bus_cu_ri_out.current_value;
    seq_output_bin = concatenate(seq_output_bin, last_bus_flags_out.current_value % 10);  // fz
    seq_output_bin = concatenate(seq_output_bin, last_bus_flags_out.current_value / 10);  // fc

    publish_message_to(CU_SEQ_JOINER_OUTPUT_BUS_TOPIC, seq_output_bin);
}

void shutdown_cu_seq_b_jnr(void) {
    unsubscribe_for(last_cu_ri_output_subscription);
    unsubscribe_for(flags_out_bus_topic_subscription);
}