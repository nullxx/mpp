/*
 * File: /src/lib/components/hl.c
 * Project: mpp-cpu
 * File Created: Sunday, 27th March 2022 10:53:23 pm
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Sunday, 27th March 2022 10:54:08 pm
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */

#include "hl.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../constants.h"
#include "../error.h"
#include "../pubsub.h"
#include "../utils.h"
#include "../watcher.h"
#include "components.h"

static Register h_reg = {.bin_value = 0, .bit_length = H_REG_SIZE_BIT};
static Register l_reg = {.bin_value = 0, .bit_length = L_REG_SIZE_BIT};

static RegisterWatcher h_reg_watcher = {.name = "H", .reg = &h_reg};
static RegisterWatcher l_reg_watcher = {.name = "L", .reg = &l_reg};

static LoadBit hcar_lb = {.value = 0};

static LoadBit lcar_lb = {.value = 0};
static PubSubSubscription *data_bus_topic_subscription = NULL;
static DataBus_t last_bus_data;

static void on_bus_data_message(PubSubMessage m) { last_bus_data = *(DataBus_t *)m.value; }

void set_hcar_lb(void) { hcar_lb.value = 1; }
void reset_hcar_lb(void) { hcar_lb.value = 0; }

void set_lcar_lb(void) { lcar_lb.value = 1; }
void reset_lcar_lb(void) { lcar_lb.value = 0; }

void init_hl(void) {
    register_watcher(&h_reg_watcher);
    register_watcher(&l_reg_watcher);

    data_bus_topic_subscription = subscribe_to(DATA_BUS_TOPIC, on_bus_data_message);
}

void shutdown_hl(void) {
    unsubscribe_for(data_bus_topic_subscription);

    unregister_watcher(&h_reg_watcher);
    unregister_watcher(&l_reg_watcher);
}

void run_hl(void) {
    if (hcar_lb.value == 1) {
        // load h
        h_reg.bin_value = last_bus_data;
    }

    if (lcar_lb.value == 1) {
        // load l
        l_reg.bin_value = last_bus_data;
    }

    char *next_hl_reg_str = (char *)malloc(sizeof(char) * (h_reg.bit_length + l_reg.bit_length + 1));
    if (next_hl_reg_str == NULL) {
        Error err = {.show_errno = false, .type = NOTICE_ERROR, .message = "Malloc error"};
        throw_error(err);
    }

    char *h_reg_bin_value_str = bin_to_str(h_reg.bin_value);
    char *l_reg_bin_value_str = bin_to_str(l_reg.bin_value);

    strcpy(next_hl_reg_str, h_reg_bin_value_str);
    strcat(next_hl_reg_str, l_reg_bin_value_str);

    free(h_reg_bin_value_str);
    free(l_reg_bin_value_str);

    unsigned long long hl_bin_value = str_to_bin(next_hl_reg_str);

    free(next_hl_reg_str);

    publish_message_to(HL_OUTPUT_BUS_TOPIC, &hl_bin_value);
}
