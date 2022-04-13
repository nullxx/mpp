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
#include "../watcher.h"
#include "components.h"
#include "../electronic/bus.h"

static LoadBit flcar_lb = {.value = 0};
static Register FC_reg = {.bit_length = FLAG_REG_SIZE_BIT, .bin_value = 0};
static Register FZ_reg = {.bit_length = FLAG_REG_SIZE_BIT, .bin_value = 0};

static RegisterWatcher FC_reg_watcher = {.name = "FC", .reg = &FC_reg};
static RegisterWatcher FZ_reg_watcher = {.name = "FZ", .reg = &FZ_reg};

static Bus_t last_bus_mxfld7_out = {.current_value = 0, .next_value = 0};
static Bus_t last_bus_mxfld0_out = {.current_value = 0, .next_value = 0};
static PubSubSubscription *mxfld7_out_bus_topic_subscription = NULL;
static PubSubSubscription *mxfld0_out_bus_topic_subscription = NULL;


void set_flcar_lb(void) { flcar_lb.value = 1; }
void reset_flcar_lb(void) { flcar_lb.value = 0; }

void init_flags(void) {
    register_watcher(&FC_reg_watcher);
    register_watcher(&FZ_reg_watcher);

    mxfld7_out_bus_topic_subscription = subscribe_to(MXFLD7_OUTPUT_BUS_TOPIC, &last_bus_mxfld7_out);
    mxfld0_out_bus_topic_subscription = subscribe_to(MXFLD0_OUTPUT_BUS_TOPIC, &last_bus_mxfld0_out);
}

void shutdown_flags(void) {
    unsubscribe_for(mxfld7_out_bus_topic_subscription);
    unsubscribe_for(mxfld0_out_bus_topic_subscription);

    unregister_watcher(&FC_reg_watcher);
    unregister_watcher(&FZ_reg_watcher);
}

static int get_flags(void) {
    int flags = 0;
    
    flags = concatenate(flags, FC_reg.bin_value);
    flags = concatenate(flags, FZ_reg.bin_value);

    return flags;
}

void run_flags(void) {
    update_bus_data(&last_bus_mxfld7_out);
    update_bus_data(&last_bus_mxfld0_out);

    int flags = get_flags();

    if (flcar_lb.value == 1) {
        // load
        // concat flags: fc|fz
        char *fc_str = itoa(last_bus_mxfld7_out.current_value);
        char *fz_str = itoa(last_bus_mxfld0_out.current_value);
        char *flags_str = str_concat(fc_str, fz_str);
        flags = atoi(flags_str);  // 00, 01, 10, 11

        FC_reg.bin_value = atoi(fc_str);
        FZ_reg.bin_value = atoi(fz_str);

        free(fc_str);
        free(fz_str);
        free(flags_str);
    }

    publish_message_to(FLAGS_OUTPUT_BUS_TOPIC, flags);
}
