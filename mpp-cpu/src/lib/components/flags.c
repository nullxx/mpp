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
#include "../electronic/bus.h"
#include "../pubsub.h"
#include "../utils.h"
#include "../watcher.h"
#include "components.h"

static Register FC_reg = {.bit_length = FLAG_REG_SIZE_BIT};
static Register FZ_reg = {.bit_length = FLAG_REG_SIZE_BIT};

static RegisterWatcher FC_reg_watcher = {.reg = &FC_reg, .type = WATCHER_TYPE_FC};
static RegisterWatcher FZ_reg_watcher = {.reg = &FZ_reg, .type = WATCHER_TYPE_FZ};

static Bus_t *last_bus_mxfld7_out = NULL;
static Bus_t *last_bus_mxfld0_out = NULL;
static Bus_t *control_bus = NULL;

static PubSubSubscription *mxfld7_out_bus_topic_subscription = NULL;
static PubSubSubscription *mxfld0_out_bus_topic_subscription = NULL;
static PubSubSubscription *control_bus_topic_subscription = NULL;

void init_flags(void) {
    initialize_word(&FC_reg.value, 0);
    initialize_word(&FZ_reg.value, 0);

    last_bus_mxfld7_out = create_bus_data();
    last_bus_mxfld0_out = create_bus_data();
    control_bus = create_bus_data();
    mxfld7_out_bus_topic_subscription = subscribe_to(MXFLD7_OUTPUT_BUS_TOPIC, last_bus_mxfld7_out);
    mxfld0_out_bus_topic_subscription = subscribe_to(MXFLD0_OUTPUT_BUS_TOPIC, last_bus_mxfld0_out);
    control_bus_topic_subscription = subscribe_to(CONTROL_BUS_TOPIC, control_bus);

    register_watcher(&FC_reg_watcher);
    register_watcher(&FZ_reg_watcher);
}

void shutdown_flags(void) {
    unregister_watcher(&FC_reg_watcher);
    unregister_watcher(&FZ_reg_watcher);

    unsubscribe_for(mxfld7_out_bus_topic_subscription);
    unsubscribe_for(mxfld0_out_bus_topic_subscription);
    unsubscribe_for(control_bus_topic_subscription);

    destroy_bus_data(last_bus_mxfld7_out);
    destroy_bus_data(last_bus_mxfld0_out);
    destroy_bus_data(control_bus);
}

static Word get_flags(Word fc, Word fz) {
    Word flags;
    initialize_word(&flags, 0);

    if (fc.bits[0] == 1) {
        flags.bits[1] = 1;
    }

    if (fz.bits[0] == 1) {
        flags.bits[0] = 1;
    }

    return flags;
}

void run_flags(void) {
    update_bus_data(last_bus_mxfld7_out);
    update_bus_data(last_bus_mxfld0_out);
    update_bus_data(control_bus);

    Word flcar_lb;
    initialize_word(&flcar_lb, 0);

    flcar_lb.bits[0] = control_bus->current_value.bits[CONTROL_BUS_FLCAR_BIT_POSITION];

    if (word_to_int(flcar_lb) == 1) {
        // load
        FC_reg.value = last_bus_mxfld7_out->current_value;
        FZ_reg.value = last_bus_mxfld0_out->current_value;
    }

    Word flags = get_flags(FC_reg.value, FZ_reg.value);

    publish_message_to(FLAGS_OUTPUT_BUS_TOPIC, flags);
}
