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

#include "../electronic/multiplexer.h"
#include "../pubsub.h"
#include "../utils.h"
#include "components.h"
#include "controllers/mxfldx.h"
#include "mxfld7.h"

static ALUFZOutputBus_t last_bus_alufz_out;
static PubSubSubscription *bus_alufz_out_subscription = NULL;

static void on_bus_alufz_out_message(PubSubMessage m) { last_bus_alufz_out = *(ALUFZOutputBus_t *)m.value; }

void init_mxfld0(void) { bus_alufz_out_subscription = subscribe_to(ALU_FZ_OUTPUT_BUS_TOPIC, on_bus_alufz_out_message); }

void shutdown_mxfld0(void) { unsubscribe_for(bus_alufz_out_subscription); }

void run_mxfld0(void) {
    char *last_bus_data_str = bin_to_str(cll_get_last_bus_data());
    const int lowbit = atoi(&last_bus_data_str[get_bin_len(cll_get_last_bus_data()) - 1]);
    free(last_bus_data_str);

    MXInput result = run_2x1_mx(cll_get_selfl_lb_value(), lowbit, last_bus_alufz_out);
    publish_message_to(MXFLD0_OUTPUT_BUS_TOPIC, &result);
}