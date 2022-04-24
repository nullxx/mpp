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

#include <stdio.h>

#include "../../electronic/bus.h"
#include "../../logger.h"
#include "../../pubsub.h"
#include "../../utils.h"

static Bus_t *last_bus_seq_jnr = NULL;
static Bus_t *actual_status_Q4_bus = NULL;
static Bus_t *actual_status_Q3_bus = NULL;
static Bus_t *actual_status_Q2_bus = NULL;
static Bus_t *actual_status_Q1_bus = NULL;
static Bus_t *actual_status_Q0_bus = NULL;
static PubSubSubscription *last_cu_seq_jnr_output_subscription;
static PubSubSubscription *actual_status_Q4_subscription = NULL;
static PubSubSubscription *actual_status_Q3_subscription = NULL;
static PubSubSubscription *actual_status_Q2_subscription = NULL;
static PubSubSubscription *actual_status_Q1_subscription = NULL;
static PubSubSubscription *actual_status_Q0_subscription = NULL;

void init_cu_seq(void) {
    last_bus_seq_jnr = create_bus_data();
    actual_status_Q4_bus = create_bus_data();
    actual_status_Q3_bus = create_bus_data();
    actual_status_Q2_bus = create_bus_data();
    actual_status_Q1_bus = create_bus_data();
    actual_status_Q0_bus = create_bus_data();

    actual_status_Q4_subscription = subscribe_to(CU_SEQ_ACTUAL_STATUS_Q4_BUS_TOPIC, actual_status_Q4_bus);
    actual_status_Q3_subscription = subscribe_to(CU_SEQ_ACTUAL_STATUS_Q3_BUS_TOPIC, actual_status_Q3_bus);
    actual_status_Q2_subscription = subscribe_to(CU_SEQ_ACTUAL_STATUS_Q2_BUS_TOPIC, actual_status_Q2_bus);
    actual_status_Q1_subscription = subscribe_to(CU_SEQ_ACTUAL_STATUS_Q1_BUS_TOPIC, actual_status_Q1_bus);
    actual_status_Q0_subscription = subscribe_to(CU_SEQ_ACTUAL_STATUS_Q0_BUS_TOPIC, actual_status_Q0_bus);
    last_cu_seq_jnr_output_subscription = subscribe_to(CU_SEQ_JOINER_OUTPUT_BUS_TOPIC, last_bus_seq_jnr);
}

void run_cu_seq(void) {
    const int q4 = actual_status_Q4_bus->next_value.bits[0];
    const int q3 = actual_status_Q3_bus->next_value.bits[0];
    const int q2 = actual_status_Q2_bus->next_value.bits[0];
    const int q1 = actual_status_Q1_bus->next_value.bits[0];
    const int q0 = actual_status_Q0_bus->next_value.bits[0];

    const int c7 = last_bus_seq_jnr->next_value.bits[9];
    const int c6 = last_bus_seq_jnr->next_value.bits[8];
    const int c5 = last_bus_seq_jnr->next_value.bits[7];
    const int c4 = last_bus_seq_jnr->next_value.bits[6];
    const int c3 = last_bus_seq_jnr->next_value.bits[5];
    const int c2 = last_bus_seq_jnr->next_value.bits[4];
    const int c1 = last_bus_seq_jnr->next_value.bits[3];
    const int c0 = last_bus_seq_jnr->next_value.bits[2];
    const int fz = last_bus_seq_jnr->next_value.bits[1];
    const int fc = last_bus_seq_jnr->next_value.bits[0];

    int found = -1;
    // FIXME - this is a hack, fix it => D4 = Q4' * Q3' * Q2' * Q1' * Q0 * C7 ...
    for (int i = 0; i < TRUTH_TABLE_ROWS; i++) {
        if (truth_table[i][1] != -1 && truth_table[i][1] != q4) continue;
        if (truth_table[i][2] != -1 && truth_table[i][2] != q3) continue;
        if (truth_table[i][3] != -1 && truth_table[i][3] != q2) continue;
        if (truth_table[i][4] != -1 && truth_table[i][4] != q1) continue;
        if (truth_table[i][5] != -1 && truth_table[i][5] != q0) continue;
        if (truth_table[i][6] != -1 && truth_table[i][6] != c7) continue;
        if (truth_table[i][7] != -1 && truth_table[i][7] != c6) continue;
        if (truth_table[i][8] != -1 && truth_table[i][8] != c5) continue;
        if (truth_table[i][9] != -1 && truth_table[i][9] != c4) continue;
        if (truth_table[i][10] != -1 && truth_table[i][10] != c3) continue;
        if (truth_table[i][11] != -1 && truth_table[i][11] != c2) continue;
        if (truth_table[i][12] != -1 && truth_table[i][12] != c1) continue;
        if (truth_table[i][13] != -1 && truth_table[i][13] != c0) continue;
        if (truth_table[i][14] != -1 && truth_table[i][14] != fz) continue;
        if (truth_table[i][15] != -1 && truth_table[i][15] != fc) continue;

        found = i;
    }

    Word d4, d3, d2, d1, d0;
    initialize_word(&d4, 0);
    initialize_word(&d3, 0);
    initialize_word(&d2, 0);
    initialize_word(&d1, 0);
    initialize_word(&d0, 0);

    if (found == -1) {
        // s0
        log_debug("Could not find a match in the truth table. s0");
        d4.bits[0] = 0;
        d3.bits[0] = 0;
        d2.bits[0] = 0;
        d1.bits[0] = 0;
        d0.bits[0] = 0;
    } else {
        log_debug("Found a match in the truth table. NEXT s%d", truth_table[found][21]);
        d4.bits[0] = truth_table[found][16];
        d3.bits[0] = truth_table[found][17];
        d2.bits[0] = truth_table[found][18];
        d1.bits[0] = truth_table[found][19];
        d0.bits[0] = truth_table[found][20];
    }

    publish_message_to(CU_SEQ_OUTPUT_D4_BUS_TOPIC, d4);
    publish_message_to(CU_SEQ_OUTPUT_D3_BUS_TOPIC, d3);
    publish_message_to(CU_SEQ_OUTPUT_D2_BUS_TOPIC, d2);
    publish_message_to(CU_SEQ_OUTPUT_D1_BUS_TOPIC, d1);
    publish_message_to(CU_SEQ_OUTPUT_D0_BUS_TOPIC, d0);
}

void shutdown_cu_seq(void) {
    unsubscribe_for(last_cu_seq_jnr_output_subscription);
    unsubscribe_for(actual_status_Q4_subscription);
    unsubscribe_for(actual_status_Q3_subscription);
    unsubscribe_for(actual_status_Q2_subscription);
    unsubscribe_for(actual_status_Q1_subscription);
    unsubscribe_for(actual_status_Q0_subscription);

    destroy_bus_data(actual_status_Q4_bus);
    destroy_bus_data(actual_status_Q3_bus);
    destroy_bus_data(actual_status_Q2_bus);
    destroy_bus_data(actual_status_Q1_bus);
    destroy_bus_data(actual_status_Q0_bus);
    destroy_bus_data(last_bus_seq_jnr);
}