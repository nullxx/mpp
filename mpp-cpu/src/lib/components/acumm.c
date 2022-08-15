/*
 * File: /src/lib/components/acumm.c
 * Project: mpp-cpu
 * File Created: Tuesday, 29th March 2022 10:19:47 pm
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Tuesday, 29th March 2022 10:20:03 pm
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */

#include "acumm.h"

#include <stdio.h>

#include "../constants.h"
#include "../definitions.h"
#include "../electronic/bus.h"
#include "../error.h"
#include "../pubsub.h"
#include "../utils.h"
#include "../watcher.h"
#include "components.h"

static Register acumm_reg = {.bit_length = ACUMM_REG_SIZE_BIT};

static RegisterWatcher acumm_reg_watcher = {.reg = &acumm_reg, .type = WATCHER_TYPE_ACUM};

static Bus_t *last_bus_data = NULL;
static Bus_t *control_bus = NULL;
static PubSubSubscription *data_bus_topic_subscription = NULL;
static PubSubSubscription *control_bus_topic_subscription = NULL;

void init_acumm(void) {
    initialize_word(&acumm_reg.value, 0);

    last_bus_data = create_bus_data();
    control_bus = create_bus_data();

    data_bus_topic_subscription = subscribe_to(DATA_BUS_TOPIC, last_bus_data);
    control_bus_topic_subscription = subscribe_to(CONTROL_BUS_TOPIC, control_bus);

    register_watcher(&acumm_reg_watcher);
}

void shutdown_acumm(void) {
    unregister_watcher(&acumm_reg_watcher);
    unsubscribe_for(data_bus_topic_subscription);
    unsubscribe_for(control_bus_topic_subscription);
    destroy_bus_data(last_bus_data);
    destroy_bus_data(control_bus);
}

void run_acumm(void) {
    update_bus_data(last_bus_data);
    update_bus_data(control_bus);

    if (control_bus->current_value.bits[CONTROL_BUS_ACCAR_BIT_POSITION] == 1) {
        // load
        if (get_used_bits(last_bus_data->current_value) > acumm_reg.bit_length) {
            Error err = {.show_errno = 0, .type = NOTICE_ERROR, .message = "Overflow attemping to load to ACUMM register"};
            return throw_error(err);
        }

        acumm_reg.value = last_bus_data->current_value;
    }

    if (control_bus->current_value.bits[CONTROL_BUS_ACBUS_BIT_POSITION] == 1) {
        // read
        publish_message_to(DATA_BUS_TOPIC, acumm_reg.value);
    }

    publish_message_to(ACUMM_OUTPUT_BUS_TOPIC, acumm_reg.value);
}