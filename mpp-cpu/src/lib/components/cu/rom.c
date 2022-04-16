/*
 * File: /src/lib/components/cu/rom.c
 * Project: mpp-cpu
 * File Created: Thursday, 14th April 2022 6:30:44 pm
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Thursday, 14th April 2022 6:31:09 pm
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */

#include "rom.h"

#include <stdio.h>

#include "../../electronic/bus.h"
#include "../../pubsub.h"
#include "../../utils.h"
#include "../../logger.h"

#define X 0

#define CU_SIGNAL_ROWS_COUNT 39
#define CU_SIGNAL_COLS_COUNT 31
#define SIGNAL_SIZE_BITS 31

static Bus_t *actual_status_Q4_bus = NULL;
static Bus_t *actual_status_Q3_bus = NULL;
static Bus_t *actual_status_Q2_bus = NULL;
static Bus_t *actual_status_Q1_bus = NULL;
static Bus_t *actual_status_Q0_bus = NULL;
static PubSubSubscription *actual_status_Q4_subscription = NULL;
static PubSubSubscription *actual_status_Q3_subscription = NULL;
static PubSubSubscription *actual_status_Q2_subscription = NULL;
static PubSubSubscription *actual_status_Q1_subscription = NULL;
static PubSubSubscription *actual_status_Q0_subscription = NULL;

// FIXME: what should be X and XX and XXX?

const int ROM[CU_SIGNAL_ROWS_COUNT][CU_SIGNAL_COLS_COUNT] = {  // this are the signals
    {X, X, X, X, X, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, X, 0, 0, 0, 0, 0, 0, 0, 0},
    {X, X, X, X, X, X, X, X, X, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, X, 0, 0, 0, 0, 0, 0, 0, 0},
    {X, X, X, 0, 0, X, X, X, X, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, X, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 1, 0, 0, 1, X, X, X, X, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, X, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 1, 0, X, X, X, X, X, X, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, X, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, X, X, X, X, X, X, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, X, X, X, X, X, X, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, X, X, X, X, X, X, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 0, 1, X, X, X, X, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 0, X, X, X, X, X, X, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, X, X, X, X, X, X, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
    {1, 0, 0, X, X, X, X, X, X, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
    {X, X, X, X, X, X, X, X, X, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, X, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, X, X, X, X, X, X, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
    {1, 0, 1, X, X, X, X, X, X, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
    {X, X, X, X, X, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, X, 0, 0, 0, 0, 0, 0, 0, 0},
    {X, X, X, X, X, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, X, 0, 1, 0, 0, 0, 0, 0, 0},
    {X, X, X, X, X, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, X, 0, 0, 1, 0, 0, 0, 0, 0},
    {X, X, X, X, X, 1, 0, X, X, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, X, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 1, 0, X, X, 1, 0, X, X, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, X, 0, 0, 0, 0, 0, 0, 0, 0},
    {X, X, X, X, X, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, X, 0, 0, 0, 0, 0, 0, 0, 0},
    {X, X, X, 1, 0, X, X, X, X, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, X, 0, 1, 0, 0, 0, 0, 0, 0},
    {X, X, X, 1, 1, X, X, X, X, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, X, 0, 0, 1, 0, 0, 0, 0, 0},
    {X, X, X, X, X, X, X, X, X, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, X, 0, 0, 0, 0, 0, 0, 0, 0},
    {X, X, X, X, X, X, X, X, X, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
    {X, X, X, X, X, X, X, X, X, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, X, 0, 0, 0, 0, 0, 0, 1, 0},
    {X, X, X, X, X, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, X, 0, 0, 0, 0, 0, 1, 0, 0},
    {X, X, X, X, X, 0, 1, X, X, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, X, 0, 0, 0, 0, 0, 0, 0, 0},
    {X, X, X, X, X, 0, 1, X, X, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, X, 0, 0, 0, 0, 0, 0, 0, 0},
    {X, X, X, X, X, 0, 1, X, X, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, X, 0, 0, 0, 0, 0, 0, 0, 0},
    {X, X, X, X, X, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, X, 0, 0, 0, 0, 0, 0, 0, 0},
    {X, X, X, X, X, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, X, 0, 0, 0, 0, 0, 1, 0, 0},
    {X, X, X, X, X, 0, 1, X, X, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, X, 0, 0, 0, 0, 1, 0, 0, 0},
    {X, X, X, X, X, 0, 1, X, X, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, X, 0, 0, 0, 1, 0, 0, 0, 0},
    {X, X, X, X, X, 0, 1, X, X, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
    {X, X, X, X, X, X, X, X, X, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, X, 0, 0, 0, 0, 0, 0, 0, 1},
    {X, X, X, X, X, 0, 1, X, X, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, X, 0, 0, 0, 0, 0, 0, 0, 0},
    {X, X, X, X, X, 0, 1, X, X, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, X, 0, 0, 0, 0, 0, 0, 0, 0},
    {X, X, X, X, X, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, X, 0, 0, 0, 0, 0, 1, 0, 0}
};

void init_cu_rom(void) {
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
}

void run_cu_rom(void) {
    Word w;
    initialize_word(&w, 0);
    w.bits[4] = actual_status_Q4_bus->next_value.bits[0];
    w.bits[3] = actual_status_Q3_bus->next_value.bits[0];
    w.bits[2] = actual_status_Q2_bus->next_value.bits[0];
    w.bits[1] = actual_status_Q1_bus->next_value.bits[0];
    w.bits[0] = actual_status_Q0_bus->next_value.bits[0];

    int rom_pos = word_to_int(w);
    log_debug("ROM pos: %d", rom_pos);

    if (rom_pos < 0 || rom_pos > CU_SIGNAL_ROWS_COUNT) return;

    Word to_send;
    initialize_word(&to_send, 0);

    // int j = CU_SIGNAL_COLS_COUNT - 1;
    for (int i = 0; i < CU_SIGNAL_COLS_COUNT; i++) {
        to_send.bits[CU_SIGNAL_COLS_COUNT-1-i] = ROM[rom_pos][i];
    }

    publish_message_to(CONTROL_BUS_TOPIC, to_send);

    update_bus_data(actual_status_Q4_bus);
    update_bus_data(actual_status_Q3_bus);
    update_bus_data(actual_status_Q2_bus);
    update_bus_data(actual_status_Q1_bus);
    update_bus_data(actual_status_Q0_bus);
}

void shutdown_cu_rom(void) {
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
}