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
#include "components.h"

static LoadBit acbus_lb = {.value = 0};
static LoadBit accar_lb = {.value = 0};

static Register acumm_reg = {.bin_value = 0, .bit_length = ACUMM_REG_SIZE_BIT};

static DataBus_t last_bus_data;
static PubSubSubscription *data_bus_topic_subscription = NULL;

static void on_bus_data_message(PubSubMessage m) { last_bus_data = (DataBus_t)m.value; }

void set_acbus_lb(void) { acbus_lb.value = 1; }
void reset_acbus_lb(void) { acbus_lb.value = 0; }

void set_accar_lb(void) { accar_lb.value = 1; }
void reset_accar_lb(void) { accar_lb.value = 0; }

void init_acumm(void) { data_bus_topic_subscription = subscribe_to(DATA_BUS_TOPIC, on_bus_data_message); }
void shutdown_acumm(void) { unsubscribe_for(data_bus_topic_subscription); }

void run_acumm(void) {
    if (accar_lb.value == 1) {
        // load
        if (get_bin_len(last_bus_data) > acumm_reg.bit_length) {
            Error err = {.show_errno = false, .type = NOTICE_ERROR, .message = "Overflow attemping to load to ACUMM register"};
            throw_error(err);
        }

        acumm_reg.bin_value = last_bus_data;
    }

    if (acbus_lb.value == 1) {
        // read
        publish_message_to(DATA_BUS_TOPIC, (void *)acumm_reg.bin_value);
    }

    publish_message_to(ACUMM_OUTPUT_BUS_TOPIC, (void *)acumm_reg.bin_value);
}