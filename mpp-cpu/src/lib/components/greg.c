/*
 * File: /src/lib/components/greg.c
 * Project: mpp-cpu
 * File Created: Wednesday, 30th March 2022 11:56:17 am
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Wednesday, 30th March 2022 11:56:22 am
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */

#include "greg.h"

#include <stdio.h>

#include "../constants.h"
#include "../error.h"
#include "../pubsub.h"
#include "../utils.h"
#include "components.h"

static LoadBit regbus_lb = {.value = 1};
static LoadBit regcar_lb = {.value = 00};
static Register A_reg = {.bin_value = 00000000, .bit_length = REG_SIZE_BIT};
static Register B_reg = {.bin_value = 00000000, .bit_length = REG_SIZE_BIT};
static Register C_reg = {.bin_value = 00000000, .bit_length = REG_SIZE_BIT};
static Register D_reg = {.bin_value = 00000000, .bit_length = REG_SIZE_BIT};
static PubSubSubscription *data_bus_topic_subscription = NULL;
static PubSubSubscription *selreg_output_bus_topic_subscription = NULL;
static DataBus_t last_bus_data;
static SelRegOutputBus_t last_bus_selreg_output;

void set_gregbus_lb(void) { regbus_lb.value = 1; }
void reset_gregbus_lb(void) { regbus_lb.value = 0; }

void set_gregcar_lb(void) { regcar_lb.value = 1; }
void reset_gregcar_lb(void) { regcar_lb.value = 1; }

static void on_bus_data_message(PubSubMessage m) { last_bus_data = *(DataBus_t *)m.value; }
static void on_bus_selreg_output_message(PubSubMessage m) { last_bus_selreg_output = *(SelRegOutputBus_t *)m.value; }

void init_greg(void) {
    data_bus_topic_subscription = subscribe_to(DATA_BUS_TOPIC, on_bus_data_message);
    selreg_output_bus_topic_subscription = subscribe_to(SELREG_OUTPUT_BUS_TOPIC, on_bus_selreg_output_message);
}

void shutdown_greg(void) {
    unsubscribe_for(data_bus_topic_subscription);
    unsubscribe_for(selreg_output_bus_topic_subscription);
}

static Register *get_register(SelRegOutputBus_t selreg) {
    switch (selreg) {
        case 00:
            return &A_reg;

        case 01:
            return &B_reg;

        case 10:
            return &C_reg;

        case 11:
            return &D_reg;

        default:
            break;
    }
    return NULL;
}

void run_greg(void) {
    Register *reg = get_register(last_bus_selreg_output);
    if (reg == NULL) {
        Error err = {.show_errno = false, .type = NOTICE_ERROR, .message = "Could not find selected register"};
        throw_error(err);
    }

    if (regcar_lb.value == 1) {
        // load to reg
        if (get_bin_len(last_bus_data) > reg->bit_length) {
            Error err = {.show_errno = false, .type = NOTICE_ERROR, .message = "Overflow attemping to load to a general register"};
            throw_error(err);
        }
        reg->bin_value = last_bus_data;
    }

    if (regbus_lb.value == 1) {
        // read to databus
        publish_message_to(DATA_BUS_TOPIC, &reg->bin_value);
    }
}