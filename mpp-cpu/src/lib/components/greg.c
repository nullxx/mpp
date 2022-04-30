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
#include "../electronic/bus.h"
#include "../error.h"
#include "../pubsub.h"
#include "../utils.h"
#include "../watcher.h"
#include "components.h"

static Register B_reg = {.bit_length = REG_SIZE_BIT};
static Register C_reg = {.bit_length = REG_SIZE_BIT};
static Register D_reg = {.bit_length = REG_SIZE_BIT};
static Register E_reg = {.bit_length = REG_SIZE_BIT};

static RegisterWatcher B_reg_watcher = {.reg = &B_reg, .type = WATCHER_TYPE_B};
static RegisterWatcher C_reg_watcher = {.reg = &C_reg, .type = WATCHER_TYPE_C};
static RegisterWatcher D_reg_watcher = {.reg = &D_reg, .type = WATCHER_TYPE_D};
static RegisterWatcher E_reg_watcher = {.reg = &E_reg, .type = WATCHER_TYPE_E};

static Bus_t *last_bus_data = NULL;
static Bus_t *last_bus_selreg_output = NULL;
static Bus_t *control_bus = NULL;

static PubSubSubscription *data_bus_topic_subscription = NULL;
static PubSubSubscription *selreg_output_bus_topic_subscription = NULL;
static PubSubSubscription *control_bus_topic_subscription = NULL;

void init_greg(void) {
    initialize_word(&B_reg.value, 0);
    initialize_word(&C_reg.value, 0);
    initialize_word(&D_reg.value, 0);
    initialize_word(&E_reg.value, 0);

    last_bus_data = create_bus_data();
    last_bus_selreg_output = create_bus_data();
    control_bus = create_bus_data();
    data_bus_topic_subscription = subscribe_to(DATA_BUS_TOPIC, last_bus_data);
    selreg_output_bus_topic_subscription = subscribe_to(SELREG_OUTPUT_BUS_TOPIC, last_bus_selreg_output);
    control_bus_topic_subscription = subscribe_to(CONTROL_BUS_TOPIC, control_bus);

    register_watcher(&B_reg_watcher);
    register_watcher(&C_reg_watcher);
    register_watcher(&D_reg_watcher);
    register_watcher(&E_reg_watcher);
}

void shutdown_greg(void) {
    unregister_watcher(&B_reg_watcher);
    unregister_watcher(&C_reg_watcher);
    unregister_watcher(&D_reg_watcher);
    unregister_watcher(&E_reg_watcher);

    unsubscribe_for(data_bus_topic_subscription);
    unsubscribe_for(selreg_output_bus_topic_subscription);
    unsubscribe_for(control_bus_topic_subscription);
    destroy_bus_data(last_bus_data);
    destroy_bus_data(last_bus_selreg_output);
    destroy_bus_data(control_bus);
}

static Register *get_gregister(int selreg) {
    switch (selreg) {
        case 0b00:
            return &B_reg;

        case 0b01:
            return &C_reg;

        case 0b10:
            return &D_reg;

        case 0b11:
            return &E_reg;

        default:
            break;
    }
    return NULL;
}

void run_greg(void) {
    update_bus_data(last_bus_data);
    update_bus_data(last_bus_selreg_output);
    update_bus_data(control_bus);

    Register *reg = get_gregister(word_to_int(last_bus_selreg_output->current_value));
    if (reg == NULL) {
        Error err = {.show_errno = false, .type = NOTICE_ERROR, .message = "Could not find selected register"};
        return throw_error(err);
    }

    Word regcar_lb;
    Word regbus_lb;
    initialize_word(&regcar_lb, 0);
    initialize_word(&regbus_lb, 0);

    regcar_lb.bits[0] = control_bus->current_value.bits[CONTROL_BUS_REGCAR_BIT_POSITION];

    regbus_lb.bits[0] = control_bus->current_value.bits[CONTROL_BUS_REGBUS_BIT_POSITION];

    if (word_to_int(regcar_lb) == 1) {
        // load to reg
        if (get_used_bits(last_bus_data->current_value) > reg->bit_length) {
            Error err = {.show_errno = false, .type = NOTICE_ERROR, .message = "Overflow attemping to load to a general register"};
            return throw_error(err);
        }
        reg->value = last_bus_data->current_value;
    }

    if (word_to_int(regbus_lb) == 1) {
        // read to databus
        publish_message_to(DATA_BUS_TOPIC, reg->value);
    }
}