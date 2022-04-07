/*
 * File: /src/lib/components/sp.c
 * Project: mpp-cpu
 * File Created: Thursday, 24th March 2022 7:12:00 pm
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Thursday, 24th March 2022 7:12:02 pm
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */

#include "sp.h"

#include <stdio.h>

#include "../constants.h"
#include "../error.h"
#include "../pubsub.h"
#include "../utils.h"
#include "../watcher.h"
#include "components.h"

static Register sp_reg = {.bin_value = 0, .bit_length = SP_REG_SIZE_BIT};

RegisterWatcher sp_reg_watcher = {.name = "SP", .reg = &sp_reg};

static LoadBit spcar_lb = {
    .value = 0,
};

static DirBus_t last_bus_dir;
static PubSubSubscription *dir_bus_topic_subscription = NULL;

static void on_bus_dir_message(PubSubMessage m) { last_bus_dir = *(DirBus_t *)m.value; }

void init_sp(void) {
    register_watcher(&sp_reg_watcher);

    dir_bus_topic_subscription = subscribe_to(DIR_BUS_TOPIC, on_bus_dir_message);
}

void shutdown_sp(void) {
    unsubscribe_for(dir_bus_topic_subscription);

    unregister_watcher(&sp_reg_watcher);
}

void set_spcar_lb(void) { spcar_lb.value = 1; }

void reset_spcar_lb(void) { spcar_lb.value = 0; }

void run_sp(void) {
    if (spcar_lb.value == 1) {  // load
        if (get_bin_len(last_bus_dir) > sp_reg.bit_length) {
            Error err = {.show_errno = false, .type = NOTICE_ERROR, .message = "Overflow attemping to load SP register"};
            throw_error(err);
        }

        sp_reg.bin_value = last_bus_dir;
    }

    // in both cases: read and after loading, we are going to transmit it
    publish_message_to(SP_OUTPUT_BUS_TOPIC, &sp_reg.bin_value);
}
