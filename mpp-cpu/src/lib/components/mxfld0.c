/*
 * File: /src/lib/components/mxfld0.c
 * Project: mpp-cpu
 * File Created: Friday, 1st April 2022 10:53:41 pm
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Friday, 1st April 2022 10:54:57 pm
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */

#include <stdio.h>
#include <stdlib.h>

#include "../electronic/bus.h"
#include "../electronic/multiplexer.h"
#include "../pubsub.h"
#include "../utils.h"
#include "components.h"
#include "controllers/mxfldx.h"
#include "mxfld7.h"

static Bus_t *last_bus_alufz_out = NULL;
static PubSubSubscription *bus_alufz_out_subscription = NULL;

void init_mxfld0(void) {
    last_bus_alufz_out = create_bus_data();
    bus_alufz_out_subscription = subscribe_to(ALU_FZ_OUTPUT_BUS_TOPIC, last_bus_alufz_out);
}

void shutdown_mxfld0(void) {
    unsubscribe_for(bus_alufz_out_subscription);
    destroy_bus_data(last_bus_alufz_out);
}

void run_mxfld0(void) {
    update_bus_data(last_bus_alufz_out);

    const int lowbit = cll_get_last_bus_data().bits[0];

    MXInput result = run_2x1_mx(cll_get_selfl_lb_value(), lowbit, word_to_int(last_bus_alufz_out->current_value));
    publish_message_to(MXFLD0_OUTPUT_BUS_TOPIC, int_to_word(result));
}