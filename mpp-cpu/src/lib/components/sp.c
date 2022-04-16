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
#include "../electronic/bus.h"
#include "../error.h"
#include "../pubsub.h"
#include "../utils.h"
#include "../watcher.h"
#include "components.h"

static Register sp_reg = {.bit_length = SP_REG_SIZE_BIT};

RegisterWatcher sp_reg_watcher = {.name = "SP", .reg = &sp_reg};

static Bus_t *last_bus_dir = NULL;
static Bus_t *control_bus = NULL;

static PubSubSubscription *dir_bus_topic_subscription = NULL;
static PubSubSubscription *control_bus_topic_subscription = NULL;

void init_sp(void) {
    initialize_word(&sp_reg.value, 0);

    last_bus_dir = create_bus_data();
    control_bus = create_bus_data();
    dir_bus_topic_subscription = subscribe_to(DIR_BUS_TOPIC_1, last_bus_dir);
    control_bus_topic_subscription = subscribe_to(CONTROL_BUS_TOPIC, control_bus);
    register_watcher(&sp_reg_watcher);
}

void shutdown_sp(void) {
    unregister_watcher(&sp_reg_watcher);
    unsubscribe_for(dir_bus_topic_subscription);
    unsubscribe_for(control_bus_topic_subscription);

    destroy_bus_data(last_bus_dir);
    destroy_bus_data(control_bus);
}

void run_sp(void) {
    update_bus_data(last_bus_dir);
    update_bus_data(control_bus);

    Word spcar_lb;
    initialize_word(&spcar_lb, 0);

    spcar_lb.bits[0] = control_bus->current_value.bits[CONTROL_BUS_SPCAR_BIT_POSITION];

    if (word_to_int(spcar_lb) == 1) {  // load
        if (get_used_bits(word_to_int(last_bus_dir->current_value)) > sp_reg.bit_length) {
            Error err = {.show_errno = 0, .type = NOTICE_ERROR, .message = "Overflow attemping to load SP register"};
            throw_error(err);
        }

        sp_reg.value = last_bus_dir->current_value;
    }

    // in both cases: read and after loading, we are going to transmit it
    publish_message_to(SP_OUTPUT_BUS_TOPIC, sp_reg.value);
}
