/*
 * File: /src/lib/components/cu/seq.c
 * Project: mpp-cpu
 * File Created: Thursday, 14th April 2022 12:58:57 pm
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Thursday, 14th April 2022 12:59:47 pm
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */
#include "seq.h"

#include "../../electronic/bus.h"
#include "../../pubsub.h"
#include "../../utils.h"

Bus_t last_bus_seq_jnr = {.current_value = 0, .next_value = 0};
PubSubSubscription *last_cu_seq_jnr_output_subscription;

void init_cu_seq(void) { last_cu_seq_jnr_output_subscription = subscribe_to(CU_SEQ_JOINER_OUTPUT_BUS_TOPIC, &last_bus_seq_jnr); }

void run_cu_seq(void) {
    update_bus_data(&last_bus_seq_jnr);

    const int c7 = digit(last_bus_seq_jnr.current_value, 10);
    const int c6 = digit(last_bus_seq_jnr.current_value, 9);
    const int c5 = digit(last_bus_seq_jnr.current_value, 8);
    const int c4 = digit(last_bus_seq_jnr.current_value, 7);
    const int c3 = digit(last_bus_seq_jnr.current_value, 6);
    const int c2 = digit(last_bus_seq_jnr.current_value, 5);
    const int c1 = digit(last_bus_seq_jnr.current_value, 4);
    const int c0 = digit(last_bus_seq_jnr.current_value, 3);

    const int fz = digit(last_bus_seq_jnr.current_value, 2);
    const int fc = digit(last_bus_seq_jnr.current_value, 1);

    // TODO - this is a hack, fix it => D4 = Q4' * Q3' * Q2' * Q1' * Q0 * C7 ...
    for (int i = 0; i < TRUTH_TABLE_ROWS; i++) {
        // if (truth_table[i][0] == )
    }
}