/*
 * File: /src/lib/components/mxfld7.c
 * Project: mpp-cpu
 * File Created: Friday, 1st April 2022 10:53:41 pm
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Friday, 1st April 2022 10:54:57 pm
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */

#include "mxfld7.h"

#include <stdio.h>
#include <stdlib.h>

#include "../electronic/multiplexer.h"
#include "../pubsub.h"
#include "../utils.h"
#include "components.h"
#include "controllers/mxfldx.h"

static ALUFCOutputBus_t last_bus_alufc_out;
static PubSubSubscription *bus_alufc_out_subscription = NULL;

static void on_bus_alufc_out_message(PubSubMessage m) { last_bus_alufc_out = *(ALUFCOutputBus_t *)m.value; }

void init_mxfld7(void) {
    bus_alufc_out_subscription = subscribe_to(ALU_FC_OUTPUT_BUS_TOPIC, on_bus_alufc_out_message);
}

void shutdown_mxfld7(void) { unsubscribe_for(bus_alufc_out_subscription); }

void run_mxfld7(void) {
    char *last_bus_data_str = bin_to_str(cll_get_last_bus_data());
    const int hibit = atoi(&last_bus_data_str[0]);
    free(last_bus_data_str);

    MXInput result = run_2x1_mx(cll_get_selfl_lb_value(), hibit, last_bus_alufc_out);
    publish_message_to(MXFLD7_OUTPUT_BUS_TOPIC, &result);
}