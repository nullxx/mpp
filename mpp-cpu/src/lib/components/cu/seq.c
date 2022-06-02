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

#define TRUTH_TABLE_ROWS 100
#define TRUTH_TABLE_COLS 24

#include "states.h"
#define X -1

static int truth_table[TRUTH_TABLE_ROWS][TRUTH_TABLE_COLS] = {
    {S0, 0, 0, 0, 0, 0, 0, X, X, X, X, X, X, X, X, X, X, 0, 0, 0, 0, 0, 1, S1},
    {S1, 0, 0, 0, 0, 0, 1, 0, 0, X, X, X, X, X, X, X, X, 0, 0, 0, 0, 1, 0, S2},
    {S1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, X, X, X, X, X, X, 0, 0, 1, 1, 0, 0, S12},
    {S1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, X, X, X, X, X, X, 0, 0, 1, 1, 1, 1, S15},
    {S1, 0, 0, 0, 0, 0, 1, 1, 0, X, 1, X, X, X, X, X, X, 0, 1, 0, 1, 0, 1, S21},
    {S1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, X, X, X, X, 0, 1, 0, 0, 0, 0, S16},
    {S1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, X, X, 0, 1, 0, 0, 0, 0, S16},
    {S1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, X, X, 0, 1, 0, 0, 0, 0, S16},
    {S1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, X, X, 0, 1, 0, 0, 0, 0, S16},
    {S1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, X, X, 0, 0, 1, 1, 1, 0, S14},
    {S1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, X, X, 0, 1, 0, 1, 1, 1, S23},
    {S1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, X, X, 0, 1, 1, 0, 0, 0, S24},
    {S1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, X, X, 0, 1, 1, 0, 1, 0, S26},
    {S1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, X, X, 1, 0, 0, 0, 0, 0, S32},
    {S1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, X, X, 1, 0, 0, 0, 0, 0, S32},
    {S1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, X, X, 1, 0, 0, 1, 0, 1, S37},
    {S2, 0, 0, 0, 0, 1, 0, X, X, 0, 0, X, X, X, X, X, X, 0, 0, 0, 0, 1, 1, S3},
    {S2, 0, 0, 0, 0, 1, 0, X, X, 0, 1, 0, 0, X, X, X, X, 0, 0, 0, 1, 0, 0, S4},
    {S2, 0, 0, 0, 0, 1, 0, X, X, 0, 1, 1, 0, X, X, X, X, 0, 0, 0, 1, 1, 0, S6},
    {S2, 0, 0, 0, 0, 1, 0, X, X, 0, 1, 1, 1, X, X, X, X, 0, 0, 0, 1, 1, 1, S7},
    {S2, 0, 0, 0, 0, 1, 0, X, X, 1, 0, 0, 0, X, X, X, X, 0, 0, 1, 0, 0, 1, S9},
    {S2, 0, 0, 0, 0, 1, 0, X, X, 1, 0, 0, 1, X, X, X, X, 0, 0, 1, 0, 1, 0, S10},
    {S2, 0, 0, 0, 0, 1, 0, X, X, 1, 0, 1, 0, X, X, X, X, 0, 0, 1, 0, 1, 1, S11},
    {S2, 0, 0, 0, 0, 1, 0, X, X, 1, 0, 1, 1, X, X, X, X, 0, 0, 1, 0, 0, 0, S8},
    {S2, 0, 0, 0, 0, 1, 0, X, X, 1, 1, X, X, X, X, X, X, 0, 0, 0, 1, 0, 1, S5},
    {S3, 0, 0, 0, 0, 1, 1, X, X, X, X, X, X, X, X, X, X, 0, 0, 0, 0, 0, 0, S0},
    {S4, 0, 0, 0, 1, 0, 0, X, X, X, X, X, X, X, X, X, X, 0, 0, 0, 0, 0, 0, S0},
    {S5, 0, 0, 0, 1, 0, 1, X, X, X, X, X, X, X, X, X, X, 0, 0, 0, 0, 0, 0, S0},
    {S6, 0, 0, 0, 1, 1, 0, X, X, X, X, X, X, X, X, X, X, 0, 0, 0, 0, 0, 0, S0},
    {S7, 0, 0, 0, 1, 1, 1, X, X, X, X, X, X, X, X, X, X, 0, 0, 0, 0, 0, 0, S0},
    {S8, 0, 0, 1, 0, 0, 0, X, X, X, X, X, X, X, X, X, X, 0, 0, 0, 0, 0, 0, S0},
    {S9, 0, 0, 1, 0, 0, 1, X, X, X, X, X, X, X, X, X, X, 0, 0, 0, 0, 0, 0, S0},
    {S10, 0, 0, 1, 0, 1, 0, X, X, X, X, X, X, X, X, X, X, 0, 0, 0, 0, 0, 0, S0},
    {S11, 0, 0, 1, 0, 1, 1, X, X, X, X, X, X, X, X, X, X, 0, 0, 0, 0, 0, 0, S0},
    {S12, 0, 0, 1, 1, 0, 0, X, X, X, 0, 0, 0, X, X, X, X, 1, 0, 1, 1, 0, 0, S44},
    {S12, 0, 0, 1, 1, 0, 0, X, X, X, 0, 0, 1, 0, 0, X, X, 0, 0, 0, 1, 0, 0, S4},
    {S12, 0, 0, 1, 1, 0, 0, X, X, X, 0, 0, 1, 0, 1, X, X, 0, 0, 0, 1, 0, 1, S5},
    {S12, 0, 0, 1, 1, 0, 0, X, X, X, 0, 0, 1, 1, 0, X, X, 0, 0, 0, 1, 1, 0, S6},
    {S12, 0, 0, 1, 1, 0, 0, X, X, X, 0, 0, 1, 1, 1, X, X, 0, 0, 0, 1, 1, 1, S7},
    {S12, 0, 0, 1, 1, 0, 0, X, X, X, 0, 1, X, 0, 0, X, X, 0, 0, 1, 0, 0, 1, S9},
    {S12, 0, 0, 1, 1, 0, 0, X, X, X, 0, 1, X, 0, 1, X, X, 0, 0, 1, 0, 1, 0, S10},
    {S12, 0, 0, 1, 1, 0, 0, X, X, X, 0, 1, X, 1, 0, X, X, 0, 0, 1, 0, 1, 1, S11},
    {S12, 0, 0, 1, 1, 0, 0, X, X, X, 0, 1, X, 1, 1, X, X, 0, 0, 1, 1, 0, 1, S13},
    {S12, 0, 0, 1, 1, 0, 0, X, X, X, 1, X, X, X, X, X, X, 0, 1, 0, 0, 1, 1, S19},
    {S13, 0, 0, 1, 1, 0, 1, X, X, X, X, X, X, X, X, X, X, 0, 0, 0, 0, 0, 0, S0},
    {S14, 0, 0, 1, 1, 1, 0, X, X, X, X, X, X, X, X, X, X, 0, 0, 0, 0, 0, 0, S0},
    {S15, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, X, X, X, X, 1, 0, 0, 1, 1, 1, S39}, // NEW that points to an S3 clone changing MXREG
    {S15, 0, 0, 1, 1, 1, 1, X, X, X, X, 0, 1, 0, 0, X, X, 0, 0, 0, 1, 0, 0, S4},
    {S15, 0, 0, 1, 1, 1, 1, X, X, X, X, 0, 1, 0, 1, X, X, 0, 0, 0, 1, 0, 1, S5},
    {S15, 0, 0, 1, 1, 1, 1, X, X, X, X, 0, 1, 1, 0, X, X, 0, 0, 0, 1, 1, 0, S6},
    {S15, 0, 0, 1, 1, 1, 1, X, X, X, X, 0, 1, 1, 1, X, X, 0, 0, 0, 1, 1, 1, S7},
    {S15, 0, 0, 1, 1, 1, 1, X, X, X, X, 1, X, 0, 0, X, X, 0, 0, 1, 0, 0, 1, S9},
    {S15, 0, 0, 1, 1, 1, 1, X, X, X, X, 1, X, 0, 1, X, X, 0, 0, 1, 0, 1, 0, S10},
    {S15, 0, 0, 1, 1, 1, 1, X, X, X, X, 1, X, 1, 0, X, X, 0, 0, 1, 0, 1, 1, S11},
    {S16, 0, 1, 0, 0, 0, 0, X, X, X, X, X, X, X, X, X, X, 0, 1, 0, 0, 0, 1, S17},
    {S17, 0, 1, 0, 0, 0, 1, X, X, X, X, X, 0, 0, 0, X, X, 0, 1, 0, 0, 1, 0, S18},
    {S17, 0, 1, 0, 0, 0, 1, X, X, X, X, X, 0, 0, 1, X, X, 0, 0, 1, 1, 0, 0, S12},
    {S17, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 1, X, 0, 1, 0, 1, 0, 0, S20},
    {S17, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 1, X, 1, 0, 1, 0, 1, 0, 0, S20},
    {S17, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, X, X, 0, 1, 0, 1, 0, 0, S20},
    {S17, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, X, X, 1, 0, 0, 1, 1, 0, S38},
    {S17, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, X, X, 0, 1, 1, 0, 1, 0, S26},
    {S17, 0, 1, 0, 0, 0, 1, X, X, X, X, X, 0, 1, 0, 0, X, 0, 0, 0, 0, 0, 0, S0},
    {S17, 0, 1, 0, 0, 0, 1, X, X, X, X, X, 0, 1, 1, X, 0, 0, 0, 0, 0, 0, 0, S0},
    {S18, 0, 1, 0, 0, 1, 0, X, X, X, X, X, X, X, X, X, X, 0, 0, 0, 1, 0, 0, S4},
    {S19, 0, 1, 0, 0, 1, 1, X, X, X, X, X, X, X, X, X, X, 0, 0, 0, 0, 0, 0, S0},
    {S20, 0, 1, 0, 1, 0, 0, X, X, X, X, X, X, X, X, X, X, 0, 0, 0, 0, 0, 0, S0},
    {S21, 0, 1, 0, 1, 0, 1, X, X, X, X, X, X, X, X, X, X, 0, 1, 0, 1, 1, 0, S22},
    {S21, 0, 1, 0, 1, 1, 0, X, X, 0, X, X, X, X, X, X, X, 0, 0, 1, 1, 0, 0, S12},
    {S22, 0, 1, 0, 1, 1, 0, X, X, 1, X, X, X, X, X, X, X, 0, 1, 0, 0, 1, 0, S18},
    {S23, 0, 1, 0, 1, 1, 1, X, X, X, X, X, X, X, X, X, X, 0, 0, 0, 0, 0, 0, S0},
    {S24, 0, 1, 1, 0, 0, 0, X, X, X, X, X, X, X, X, X, X, 0, 0, 0, 0, 0, 0, S0},
    {S25, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, X, X, 0, 1, 1, 0, 1, 0, S26},
    {S26, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, X, X, 1, 0, 0, 1, 0, 0, S36},
    {S26, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, X, X, 0, 1, 1, 1, 0, 0, S28},
    {S26, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, X, X, 0, 1, 1, 1, 0, 1, S27},
    {S27, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, X, X, 1, 0, 1, 0, 0, 0, S40},
    {S28, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, X, X, 1, 0, 1, 0, 0, 0, S40},
    {S28, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, X, X, 1, 0, 1, 0, 1, 0, S42},
    {S29, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, X, X, 0, 1, 0, 1, 0, 0, S20},
    {S29, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, X, X, 0, 1, 1, 1, 1, 0, S30},
    {S30, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, X, X, 0, 0, 0, 0, 0, 0, S0},
    {S31, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, X, X, 1, 0, 0, 0, 0, 1, S33},
    {S31, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, X, X, 1, 0, 0, 0, 0, 1, S33},
    {S32, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, X, X, 0, 1, 1, 1, 1, 1, S31},
    {S32, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, X, X, 0, 1, 1, 1, 1, 1, S31},
    {S33, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, X, X, 1, 0, 1, 0, 0, 1, S41},
    {S33, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, X, X, 1, 0, 1, 0, 0, 1, S41},
    {S34, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, X, X, 1, 0, 1, 0, 1, 1, S43},
    {S35, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, X, X, 0, 0, 0, 0, 0, 0, S0},
    {S36, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, X, X, 0, 0, 0, 0, 0, 0, S0},
    {S37, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, X, X, 0, 1, 1, 1, 1, 1, S31},
    {S38, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, X, X, 0, 0, 0, 0, 0, 0, S0},
    {S40, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, X, X, 0, 1, 1, 1, 0, 1, S29},
    {S40, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, X, X, 0, 1, 1, 1, 0, 0, S28},
    {S41, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, X, X, 0, 0, 0, 0, 0, 0, S0},
    {S41, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, X, X, 1, 0, 0, 0, 1, 0, S34},
    {S42, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, X, X, 0, 1, 1, 1, 0, 1, S29},
    {S43, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, X, X, 1, 0, 0, 0, 1, 1, S35},
    {S44, 1, 0, 1, 1, 0, 0, X, X, X, 0, 0, 0, X, X, X, X, 0, 0, 0, 0, 0, 0, S0},
};


static Bus_t *last_bus_seq_jnr = NULL;
static Bus_t *actual_status_Q5_bus = NULL;
static Bus_t *actual_status_Q4_bus = NULL;
static Bus_t *actual_status_Q3_bus = NULL;
static Bus_t *actual_status_Q2_bus = NULL;
static Bus_t *actual_status_Q1_bus = NULL;
static Bus_t *actual_status_Q0_bus = NULL;
static PubSubSubscription *last_cu_seq_jnr_output_subscription;
static PubSubSubscription *actual_status_Q5_subscription = NULL;
static PubSubSubscription *actual_status_Q4_subscription = NULL;
static PubSubSubscription *actual_status_Q3_subscription = NULL;
static PubSubSubscription *actual_status_Q2_subscription = NULL;
static PubSubSubscription *actual_status_Q1_subscription = NULL;
static PubSubSubscription *actual_status_Q0_subscription = NULL;

void init_cu_seq(void) {
    last_bus_seq_jnr = create_bus_data();
    actual_status_Q5_bus = create_bus_data();
    actual_status_Q4_bus = create_bus_data();
    actual_status_Q3_bus = create_bus_data();
    actual_status_Q2_bus = create_bus_data();
    actual_status_Q1_bus = create_bus_data();
    actual_status_Q0_bus = create_bus_data();

    actual_status_Q5_subscription = subscribe_to(CU_SEQ_ACTUAL_STATUS_Q5_BUS_TOPIC, actual_status_Q5_bus);
    actual_status_Q4_subscription = subscribe_to(CU_SEQ_ACTUAL_STATUS_Q4_BUS_TOPIC, actual_status_Q4_bus);
    actual_status_Q3_subscription = subscribe_to(CU_SEQ_ACTUAL_STATUS_Q3_BUS_TOPIC, actual_status_Q3_bus);
    actual_status_Q2_subscription = subscribe_to(CU_SEQ_ACTUAL_STATUS_Q2_BUS_TOPIC, actual_status_Q2_bus);
    actual_status_Q1_subscription = subscribe_to(CU_SEQ_ACTUAL_STATUS_Q1_BUS_TOPIC, actual_status_Q1_bus);
    actual_status_Q0_subscription = subscribe_to(CU_SEQ_ACTUAL_STATUS_Q0_BUS_TOPIC, actual_status_Q0_bus);
    last_cu_seq_jnr_output_subscription = subscribe_to(CU_SEQ_JOINER_OUTPUT_BUS_TOPIC, last_bus_seq_jnr);
}

void run_cu_seq(void) {
    const int q5 = actual_status_Q5_bus->next_value.bits[0];
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
    for (int i = 0; i < TRUTH_TABLE_ROWS; i++) {
        if (truth_table[i][1] != X && truth_table[i][1] != q5) continue;
        if (truth_table[i][2] != X && truth_table[i][2] != q4) continue;
        if (truth_table[i][3] != X && truth_table[i][3] != q3) continue;
        if (truth_table[i][4] != X && truth_table[i][4] != q2) continue;
        if (truth_table[i][5] != X && truth_table[i][5] != q1) continue;
        if (truth_table[i][6] != X && truth_table[i][6] != q0) continue;
        if (truth_table[i][7] != X && truth_table[i][7] != c7) continue;
        if (truth_table[i][8] != X && truth_table[i][8] != c6) continue;
        if (truth_table[i][9] != X && truth_table[i][9] != c5) continue;
        if (truth_table[i][10] != X && truth_table[i][10] != c4) continue;
        if (truth_table[i][11] != X && truth_table[i][11] != c3) continue;
        if (truth_table[i][12] != X && truth_table[i][12] != c2) continue;
        if (truth_table[i][13] != X && truth_table[i][13] != c1) continue;
        if (truth_table[i][14] != X && truth_table[i][14] != c0) continue;
        if (truth_table[i][15] != X && truth_table[i][15] != fz) continue;
        if (truth_table[i][16] != X && truth_table[i][16] != fc) continue;

        found = i;

        // it **should** be only one match, so for "speed" purposes I will take the first (first and last) and stop checking more
        break;
    }

    Word d5, d4, d3, d2, d1, d0;
    initialize_word(&d5, 0);
    initialize_word(&d4, 0);
    initialize_word(&d3, 0);
    initialize_word(&d2, 0);
    initialize_word(&d1, 0);
    initialize_word(&d0, 0);

    if (found == -1) {
        // s0
        log_debug("Could not find a match in the truth table. s0");
        d5.bits[0] = 0;
        d4.bits[0] = 0;
        d3.bits[0] = 0;
        d2.bits[0] = 0;
        d1.bits[0] = 0;
        d0.bits[0] = 0;
    } else {
        log_debug("Found a match in the truth table. NEXT s%d", truth_table[found][21]);
        d5.bits[0] = truth_table[found][17];
        d4.bits[0] = truth_table[found][18];
        d3.bits[0] = truth_table[found][19];
        d2.bits[0] = truth_table[found][20];
        d1.bits[0] = truth_table[found][21];
        d0.bits[0] = truth_table[found][22];
    }

    publish_message_to(CU_SEQ_OUTPUT_D5_BUS_TOPIC, d5);
    publish_message_to(CU_SEQ_OUTPUT_D4_BUS_TOPIC, d4);
    publish_message_to(CU_SEQ_OUTPUT_D3_BUS_TOPIC, d3);
    publish_message_to(CU_SEQ_OUTPUT_D2_BUS_TOPIC, d2);
    publish_message_to(CU_SEQ_OUTPUT_D1_BUS_TOPIC, d1);
    publish_message_to(CU_SEQ_OUTPUT_D0_BUS_TOPIC, d0);
}

void shutdown_cu_seq(void) {
    unsubscribe_for(last_cu_seq_jnr_output_subscription);
    unsubscribe_for(actual_status_Q5_subscription);
    unsubscribe_for(actual_status_Q4_subscription);
    unsubscribe_for(actual_status_Q3_subscription);
    unsubscribe_for(actual_status_Q2_subscription);
    unsubscribe_for(actual_status_Q1_subscription);
    unsubscribe_for(actual_status_Q0_subscription);

    destroy_bus_data(actual_status_Q5_bus);
    destroy_bus_data(actual_status_Q4_bus);
    destroy_bus_data(actual_status_Q3_bus);
    destroy_bus_data(actual_status_Q2_bus);
    destroy_bus_data(actual_status_Q1_bus);
    destroy_bus_data(actual_status_Q0_bus);
    destroy_bus_data(last_bus_seq_jnr);
}