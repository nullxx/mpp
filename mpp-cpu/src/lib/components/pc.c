/*
 * File: /src/lib/components/sp.c
 * Project: mpp-cpu
 * File Created: Thursday, 25th March 2022 15:42:00 pm
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Thursday, 25th March 2022 7:12:02 pm
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */

#include "pc.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../constants.h"
#include "../electronic/bus.h"
#include "../error.h"
#include "../pubsub.h"
#include "../thread.h"
#include "../utils.h"
#include "../watcher.h"
#include "components.h"

static Register pch_reg = { .bit_length = PCH_REG_SIZE_BIT};
static Register pcl_reg = { .bit_length = PCL_REG_SIZE_BIT};
static Register pc_reg = { .bit_length = PC_REG_SIZE_BIT};

RegisterWatcher pch_reg_watcher = {.name = "PCH", .reg = &pch_reg};
RegisterWatcher pcl_reg_watcher = {.name = "PCL", .reg = &pcl_reg};
RegisterWatcher pc_reg_watcher = {.name = "PC", .reg = &pc_reg};

static LoadBit pchcar_lb = {.value = 0};
static LoadBit pclcar_lb = {.value = 0};
static LoadBit pccar_lb = {.value = 0};

static LoadBit pch_bus = {.value = 0};
static LoadBit pcl_bus = {.value = 0};

static Bus_t *last_bus_dir = NULL;
static Bus_t *last_bus_data = NULL;
static PubSubSubscription *dir_bus_topic_subscription = NULL;
static PubSubSubscription *data_bus_topic_subscription = NULL;

void init_pc(void) {
    initialize_word(&pch_reg.value, 0);
    initialize_word(&pcl_reg.value, 0);
    initialize_word(&pc_reg.value, 0);

    last_bus_dir = create_bus_data();
    last_bus_data = create_bus_data();
    dir_bus_topic_subscription = subscribe_to(DIR_BUS_TOPIC_1, last_bus_dir);
    data_bus_topic_subscription = subscribe_to(DATA_BUS_TOPIC, last_bus_data);

    register_watcher(&pch_reg_watcher);
    register_watcher(&pcl_reg_watcher);
    register_watcher(&pc_reg_watcher);
}

void shutdown_pc(void) {
    unregister_watcher(&pch_reg_watcher);
    unregister_watcher(&pcl_reg_watcher);
    unregister_watcher(&pc_reg_watcher);

    unsubscribe_for(dir_bus_topic_subscription);
    unsubscribe_for(data_bus_topic_subscription);
    destroy_bus_data(last_bus_dir);
    destroy_bus_data(last_bus_data);
}

void set_pchbus_lb(void) { pch_bus.value = 1; }
void reset_pchbus_lb(void) { pch_bus.value = 0; }

void set_pclbus_lb(void) { pcl_bus.value = 1; }
void reset_pclbus_lb(void) { pcl_bus.value = 0; }

void set_pchcar_lb(void) { pchcar_lb.value = 1; }
void reset_pchcar_lb(void) { pchcar_lb.value = 0; }

void set_pclcar_lb(void) { pclcar_lb.value = 1; }
void reset_pclcar_lb(void) { pclcar_lb.value = 0; }

void set_pccar_lb(void) { pccar_lb.value = 1; }
void reset_pccar_lb(void) { pccar_lb.value = 0; }

void run_pc(void) {
    update_bus_data(last_bus_data);
    update_bus_data(last_bus_dir);

    // pch
    if (pchcar_lb.value == 1) {
        // load pch
        if (get_used_bits(word_to_int(last_bus_data->current_value)) > pch_reg.bit_length) {
            Error err = {.show_errno = 0, .type = NOTICE_ERROR, .message = "Overflow attemping to load PCH register"};
            throw_error(err);
        }

        pch_reg.value = last_bus_data->current_value;
    } else if (pch_bus.value == 1) {  // pchcar_lb.value = 0
        // read to data bus if pchbus enabled
        publish_message_to(DATA_BUS_TOPIC, pch_reg.value);
    }
    // pcl
    if (pchcar_lb.value == 1) {
        // load pcl
        if (get_used_bits(word_to_int(last_bus_data->current_value)) > pcl_reg.bit_length) {
            Error err = {.show_errno = 0, .type = NOTICE_ERROR, .message = "Overflow attemping to load PCL register"};
            throw_error(err);
        }

        pcl_reg.value = last_bus_data->current_value;
    } else if (pcl_bus.value == 1) {  // pchcar_lb.value = 0
        // read to data bus if pchbus enabled
        publish_message_to(DATA_BUS_TOPIC, pcl_reg.value);
    }

    // mix pch + pcl => pc. If pc is set later it will be overwritten
    if (pchcar_lb.value == 1 && pclcar_lb.value == 1) {
        // PC = PCH|PCL
        for (int i = 0; i < pcl_reg.bit_length; i++) {
            pc_reg.value.bits[i] = pcl_reg.value.bits[i];
        }

        for (int i = 0; i < pch_reg.bit_length; i++) {
            pc_reg.value.bits[pcl_reg.bit_length + i] = pch_reg.value.bits[i];
        }
    }

    // pc
    if (pccar_lb.value == 1) {
        // load pc // PC is prioritary than pch and pcl
        if (get_used_bits(word_to_int(last_bus_dir->current_value)) > pc_reg.bit_length) {
            Error err = {.show_errno = 0, .type = NOTICE_ERROR, .message = "Overflow attemping to load PC register"};
            throw_error(err);
            return;
        }

        pc_reg.value = last_bus_dir->current_value;

        for (int i = 0; i < pcl_reg.bit_length; i++) {
            pcl_reg.value.bits[i] = pc_reg.value.bits[i];
        }

        for (int i = 0; i < pch_reg.bit_length; i++) {
            pch_reg.value.bits[i] = pc_reg.value.bits[pcl_reg.bit_length + i];
        }
    }

    publish_message_to(PC_OUTPUT_BUS_TOPIC, pc_reg.value);
}
