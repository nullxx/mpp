/*
 * File: /src/lib/components/flags.c
 * Project: mpp-cpu
 * File Created: Saturday, 2nd April 2022 11:00:02 am
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Saturday, 2nd April 2022 11:00:32 am
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */

#include "flags.h"

#include <stdio.h>
#include <stdlib.h>

#include "../constants.h"
#include "../pubsub.h"
#include "../utils.h"
#include "components.h"

LoadBit flcar_lb = {.value = 0};
Register FC_reg = {.bit_length = FLAG_REG_SIZE_BIT, .bin_value = 0};
Register FZ_reg = {.bit_length = FLAG_REG_SIZE_BIT, .bin_value = 0};

static MXFlD7OutputBus_t last_bus_mxfld7_out;
static MXFlD0OutputBus_t last_bus_mxfld0_out;
static PubSubSubscription *mxfld7_out_bus_topic_subscription = NULL;
static PubSubSubscription *mxfld0_out_bus_topic_subscription = NULL;

static void on_bus_mxfld7_out_message(PubSubMessage m) { last_bus_mxfld7_out = *(MXFlD7OutputBus_t *)m.value; }
static void on_bus_mxfld0_out_message(PubSubMessage m) { last_bus_mxfld0_out = *(MXFlD0OutputBus_t *)m.value; }

void set_flcar_lb(void) { flcar_lb.value = 1; }
void reset_flcar_lb(void) { flcar_lb.value = 0; }

void init_flags(void) {
    mxfld7_out_bus_topic_subscription = subscribe_to(MXFLD7_OUTPUT_BUS_TOPIC, on_bus_mxfld7_out_message);
    mxfld0_out_bus_topic_subscription = subscribe_to(MXFLD0_OUTPUT_BUS_TOPIC, on_bus_mxfld0_out_message);
}

void shutdown_flags(void) {
    unsubscribe_for(mxfld7_out_bus_topic_subscription);
    unsubscribe_for(mxfld0_out_bus_topic_subscription);
}

void run_flags(void) {
    int flags = 00;

    if (flcar_lb.value == 1) {
        // load
        // concat flags: fc|fz
        char *fc_str = itoa(last_bus_mxfld7_out);
        char *fz_str = itoa(last_bus_mxfld0_out);
        char *flags_str = str_concat(fc_str, fz_str);
        flags = atoi(flags_str);  // 00, 01, 10, 11

        free(fc_str);
        free(fz_str);
        free(flags_str);
    }

    publish_message_to(FLAGS_OUTPUT_BUS_TOPIC, &flags);
}
