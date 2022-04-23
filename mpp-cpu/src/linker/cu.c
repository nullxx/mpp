/*
 * File: /src/linker/cu.c
 * Project: mpp-cpu
 * File Created: Friday, 22nd April 2022 11:17:52 pm
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Friday, 22nd April 2022 11:17:54 pm
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */

#include "../lib/components/cu/cu.h"

#include <stdio.h>

#include "../lib/pubsub.h"
#include "linker.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

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

static int get_rom_pos(void) {
    Word w;
    initialize_word(&w, 0);
    w.bits[4] = actual_status_Q4_bus->next_value.bits[0];
    w.bits[3] = actual_status_Q3_bus->next_value.bits[0];
    w.bits[2] = actual_status_Q2_bus->next_value.bits[0];
    w.bits[1] = actual_status_Q1_bus->next_value.bits[0];
    w.bits[0] = actual_status_Q0_bus->next_value.bits[0];

    int rom_pos = word_to_int(w);
    return rom_pos;
}

void init_linker_cu(void) {
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

void shutdown_linker_cu(void) {
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

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
double run_clock_cycle(void) {
    double time = run_cu_clock_cycle();
    get_update_ui_fn()();
    return time;
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
int get_state(void) {
    return get_rom_pos();  // the rom pos indicates the state (last)
}
