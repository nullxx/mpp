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
#include "../error.h"
#include "../pubsub.h"
#include "../utils.h"
#include "../watcher.h"
#include "components.h"
#include "../electronic/bus.h"

static LoadBit acbus_lb = {.value = 0};
static LoadBit accar_lb = {.value = 0};

static Register acumm_reg = {.bin_value = 0, .bit_length = ACUMM_REG_SIZE_BIT};

static RegisterWatcher acumm_reg_watcher = {.name = "ACUM", .reg = &acumm_reg};

static Bus_t last_bus_data = {.current_value = 0, .next_value = 0};
static PubSubSubscription *data_bus_topic_subscription = NULL;

void set_acbus_lb(void) { acbus_lb.value = 1; }
void reset_acbus_lb(void) { acbus_lb.value = 0; }

void set_accar_lb(void) { accar_lb.value = 1; }
void reset_accar_lb(void) { accar_lb.value = 0; }

void init_acumm(void) {
    register_watcher(&acumm_reg_watcher);

    data_bus_topic_subscription = subscribe_to(DATA_BUS_TOPIC, &last_bus_data);
}
void shutdown_acumm(void) {
    unsubscribe_for(data_bus_topic_subscription);
    unregister_watcher(&acumm_reg_watcher);
}

void run_acumm(void) {
    update_bus_data(&last_bus_data);

    if (accar_lb.value == 1) {
        // load
        if (get_num_len(last_bus_data.current_value) > acumm_reg.bit_length) {
            Error err = {.show_errno = 0, .type = NOTICE_ERROR, .message = "Overflow attemping to load to ACUMM register"};
            throw_error(err);
        }

        acumm_reg.bin_value = last_bus_data.current_value;
    }

    if (acbus_lb.value == 1) {
        // read
        publish_message_to(DATA_BUS_TOPIC, acumm_reg.bin_value);
    }

    publish_message_to(ACUMM_OUTPUT_BUS_TOPIC, acumm_reg.bin_value);
}