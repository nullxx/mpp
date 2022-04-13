/*
 * File: /src/lib/components/controllers/mxfldx.c
 * Project: mpp-cpu
 * File Created: Friday, 1st April 2022 11:52:16 pm
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Friday, 1st April 2022 11:52:37 pm
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */

#include "mxfldx.h"

#include <stdio.h>

#include "../../pubsub.h"
#include "../components.h"
#include "../mxfld0.h"
#include "../mxfld7.h"

LoadBit selfl_lb = {.value = 0};
static Bus_t last_bus_data = {.current_value = 0, .next_value = 0};
static PubSubSubscription *bus_data_subscription = NULL;

void cll_set_selfl_lb(void) { selfl_lb.value = 1; }
void cll_reset_selfl_lb(void) { selfl_lb.value = 0; }
int cll_get_selfl_lb_value(void) { return selfl_lb.value; }
Bin cll_get_last_bus_data(void) { return last_bus_data.current_value; }

void cll_init_mxfldx(void) {
    bus_data_subscription = subscribe_to(DATA_BUS_TOPIC, &last_bus_data);

    init_mxfld7();
    init_mxfld0();
}

void cll_shutdown_mxfldx(void) {
    shutdown_mxfld7();
    shutdown_mxfld0();

    unsubscribe_for(bus_data_subscription);
}

void cll_run_mxfldx(void) {
    run_mxfld7();
    run_mxfld0();
}