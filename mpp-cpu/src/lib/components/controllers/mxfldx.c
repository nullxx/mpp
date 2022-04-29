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
#include "../../constants.h"

static Bus_t *last_bus_data = NULL;
static Bus_t *control_bus = NULL;

static PubSubSubscription *bus_data_subscription = NULL;
static PubSubSubscription *control_bus_topic_subscription = NULL;

int cll_get_selfl_lb_value(void) {
    Word selfl_lb;
    initialize_word(&selfl_lb, 0);

    selfl_lb.bits[0] = control_bus->current_value.bits[CONTROL_BUS_SELFL_BIT_POSITION];

    return word_to_int(selfl_lb);
}

Word cll_get_last_bus_data(void) { return last_bus_data->current_value; }

void cll_init_mxfldx(void) {
    last_bus_data = create_bus_data();
    control_bus = create_bus_data();
    bus_data_subscription = subscribe_to(DATA_BUS_TOPIC, last_bus_data);
    control_bus_topic_subscription = subscribe_to(CONTROL_BUS_TOPIC, control_bus);

    init_mxfld7();
    init_mxfld0();
}

void cll_shutdown_mxfldx(void) {
    unsubscribe_for(bus_data_subscription);
    unsubscribe_for(control_bus_topic_subscription);

    destroy_bus_data(last_bus_data);
    destroy_bus_data(control_bus);

    shutdown_mxfld7();
    shutdown_mxfld0();
}

void cll_run_mxfldx(void) {
    update_bus_data(control_bus);
    update_bus_data(last_bus_data);

    run_mxfld7();
    run_mxfld0();
}