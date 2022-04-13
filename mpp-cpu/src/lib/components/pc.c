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
#include "../error.h"
#include "../pubsub.h"
#include "../thread.h"
#include "../utils.h"
#include "../watcher.h"
#include "components.h"
#include "../electronic/bus.h"

static Register pch_reg = {.bin_value = 0, .bit_length = PCH_REG_SIZE_BIT};
static Register pcl_reg = {.bin_value = 0, .bit_length = PCL_REG_SIZE_BIT};
static Register pc_reg = {.bin_value = 0, .bit_length = PC_REG_SIZE_BIT};

RegisterWatcher pch_reg_watcher = {.name = "PCH", .reg = &pch_reg};
RegisterWatcher pcl_reg_watcher = {.name = "PCL", .reg = &pcl_reg};
RegisterWatcher pc_reg_watcher = {.name = "PC", .reg = &pc_reg};

static LoadBit pchcar_lb = {.value = 0};
static LoadBit pclcar_lb = {.value = 0};
static LoadBit pccar_lb = {.value = 0};

static LoadBit pch_bus = {.value = 0};
static LoadBit pcl_bus = {.value = 0};

static Bus_t last_bus_dir = {.current_value = 0, .next_value = 0};
static Bus_t last_bus_data = {.current_value = 0, .next_value = 0};
static PubSubSubscription *dir_bus_topic_subscription = NULL;
static PubSubSubscription *data_bus_topic_subscription = NULL;

void init_pc(void) {
    register_watcher(&pch_reg_watcher);
    register_watcher(&pcl_reg_watcher);
    register_watcher(&pc_reg_watcher);

    dir_bus_topic_subscription = subscribe_to(DIR_BUS_TOPIC_1, &last_bus_dir);
    data_bus_topic_subscription = subscribe_to(DATA_BUS_TOPIC, &last_bus_data);
}

void shutdown_pc(void) {
    unsubscribe_for(dir_bus_topic_subscription);
    unsubscribe_for(data_bus_topic_subscription);

    unregister_watcher(&pch_reg_watcher);
    unregister_watcher(&pcl_reg_watcher);
    unregister_watcher(&pc_reg_watcher);
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
    update_bus_data(&last_bus_data);
    update_bus_data(&last_bus_dir);

    // pch
    if (pchcar_lb.value == 1) {
        // load pch
        if (get_num_len(last_bus_data.current_value) > pch_reg.bit_length) {
            Error err = {.show_errno = 0, .type = NOTICE_ERROR, .message = "Overflow attemping to load PCH register"};
            throw_error(err);
        }

        pch_reg.bin_value = last_bus_data.current_value;
    } else if (pch_bus.value == 1) {  // pchcar_lb.value = 0
        // read to data bus if pchbus enabled
        publish_message_to(DATA_BUS_TOPIC, pch_reg.bin_value);
    }
    // pcl
    if (pchcar_lb.value == 1) {
        // load pcl
        if (get_num_len(last_bus_data.current_value) > pcl_reg.bit_length) {
            Error err = {.show_errno = 0, .type = NOTICE_ERROR, .message = "Overflow attemping to load PCL register"};
            throw_error(err);
        }

        pcl_reg.bin_value = last_bus_data.current_value;
    } else if (pcl_bus.value == 1) {  // pchcar_lb.value = 0
        // read to data bus if pchbus enabled
        publish_message_to(DATA_BUS_TOPIC, pcl_reg.bin_value);
    }

    // mix pch + pcl => pc. If pc is set later it will be overwritten
    if (pchcar_lb.value == 1 && pclcar_lb.value == 1) {
        // char *next_pc_reg_str = (char *)malloc(sizeof(char) * (pcl_reg.bit_length + pch_reg.bit_length + 1));
        // if (next_pc_reg_str == NULL) {
        //     Error err = {.show_errno = 0, .type = NOTICE_ERROR, .message = "Malloc error"};
        //     throw_error(err);
        //     return;
        // }

        // char *pch_reg_bin_value_str = bin_to_str(pch_reg.bin_value);
        // char *pcl_reg_bin_value_str = bin_to_str(pcl_reg.bin_value);

        // strcpy(next_pc_reg_str, pch_reg_bin_value_str);
        // strcat(next_pc_reg_str, pcl_reg_bin_value_str);

        // free(pch_reg_bin_value_str);
        // free(pcl_reg_bin_value_str);

        pc_reg.bin_value = concatenate(pch_reg.bin_value, pcl_reg.bin_value);

        // free(next_pc_reg_str);
    }

    // pc
    if (pccar_lb.value == 1) {
        // load pc // PC is prioritary than pch and pcl
        if (get_num_len(last_bus_dir.current_value) > pc_reg.bit_length) {
            Error err = {.show_errno = 0, .type = NOTICE_ERROR, .message = "Overflow attemping to load PC register"};
            throw_error(err);
            return;
        }

        pc_reg.bin_value = last_bus_dir.current_value;

        pch_reg.bin_value = pc_reg.bin_value / 100000000; // 8 bits
        pcl_reg.bin_value = pc_reg.bin_value % 100000000; // 8 bits
    }

    publish_message_to(PC_OUTPUT_BUS_TOPIC, pc_reg.bin_value);
}
