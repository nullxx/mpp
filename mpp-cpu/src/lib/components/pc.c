/*
 * File: /src/lib/components/sp.c
 * Project: mpp-cpu
 * File Created: Thursday, 25th March 2022 15:42:00 pm
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Thursday, 25th March 2022 7:12:02 pm
 * Modified By: https://github.com/nullxx (mail@nullx.me>)
 */

#include "pc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../constants.h"
#include "../error.h"
#include "../pubsub.h"
#include "../utils.h"
#include "components.h"

static Register pch_reg = {.bin_value = 0, .bit_length = PCH_REG_SIZE_BIT};
static Register pcl_reg = {.bin_value = 0, .bit_length = PCL_REG_SIZE_BIT};
static Register pc_reg = {.bin_value = 0, .bit_length = PC_REG_SIZE_BIT};

static LoadBit pchcar_lb = {.value = 0};
static LoadBit pclcar_lb = {.value = 0};
static LoadBit pccar_lb = {.value = 0};

static LoadBit pch_bus = {.value = 0};
static LoadBit pcl_bus = {.value = 0};

static DirBus_t last_bus_dir;
static DataBus_t last_bus_data;
static PubSubSubscription *dir_bus_topic_subscription = NULL;
static PubSubSubscription *data_bus_topic_subscription = NULL;

static void on_bus_dir_message(PubSubMessage m) { last_bus_dir = (DirBus_t)m.value; }
static void on_bus_data_message(PubSubMessage m) { last_bus_data = (DataBus_t)m.value; }

void init_pc(void) {
    dir_bus_topic_subscription = subscribe_to(DIR_BUS_TOPIC, on_bus_dir_message);
    data_bus_topic_subscription = subscribe_to(DATA_BUS_TOPIC, on_bus_data_message);
}

void shutdown_pc(void) {
    unsubscribe_for(dir_bus_topic_subscription);
    unsubscribe_for(data_bus_topic_subscription);
}

void set_pchcar_lb(void) { pchcar_lb.value = 1; }

void reset_pchcar_lb(void) { pchcar_lb.value = 0; }

void set_pclcar_lb(void) { pclcar_lb.value = 1; }

void reset_pclcar_lb(void) { pclcar_lb.value = 0; }

void set_pccar_lb(void) { pccar_lb.value = 1; }

void reset_pccar_lb(void) { pccar_lb.value = 0; }

void run_pc(void) {
    // pch
    if (pchcar_lb.value == 1) {
        // load pch
        if (get_bin_len(last_bus_data) > pch_reg.bit_length) {
            Error err = {.show_errno = false, .type = NOTICE_ERROR, .message = "Overflow attemping to load PCH register"};
            throw_error(err);
        }

        pch_reg.bin_value = last_bus_data;
    } else if (pch_bus.value == 1) {  // pchcar_lb.value = 0
        // read to data bus if pchbus enabled
        publish_message_to(DATA_BUS_TOPIC, (void *)pch_reg.bin_value);
    }
    // pcl
    if (pchcar_lb.value == 1) {
        // load pcl
        if (get_bin_len(last_bus_data) > pcl_reg.bit_length) {
            Error err = {.show_errno = false, .type = NOTICE_ERROR, .message = "Overflow attemping to load PCL register"};
            throw_error(err);
        }

        pcl_reg.bin_value = last_bus_data;
    } else if (pcl_bus.value == 1) {  // pchcar_lb.value = 0
        // read to data bus if pchbus enabled
        publish_message_to(DATA_BUS_TOPIC, (void *)pcl_reg.bin_value);
    }

    // mix pch + pcl => pc. If pc is set later it will be overwritten
    char *next_pc_reg_str = (char *) malloc(sizeof(char) * (pcl_reg.bit_length + pch_reg.bit_length));
    if (next_pc_reg_str == NULL) {
        Error err = {
            .show_errno = false,
            .type = NOTICE_ERROR,
            .message = "Malloc error"
        };
        throw_error(err);
    }
    
    char *pch_reg_bin_value_str = bin_to_str(pch_reg.bin_value);
    char *pcl_reg_bin_value_str = bin_to_str(pcl_reg.bin_value);

    strcpy(next_pc_reg_str, pch_reg_bin_value_str);
    strcat(next_pc_reg_str, pcl_reg_bin_value_str);

    free(pch_reg_bin_value_str);
    free(pcl_reg_bin_value_str);

    pc_reg.bin_value = str_to_bin(next_pc_reg_str);

    free(next_pc_reg_str);

    // pc
    if (pccar_lb.value == 1) {
        // load pc // PC is prioritary than pch and pcl
        if (get_bin_len(last_bus_dir) > pc_reg.bit_length) {
            Error err = {.show_errno = false, .type = NOTICE_ERROR, .message = "Overflow attemping to load PC register"};
            throw_error(err);
        }

        pc_reg.bin_value = last_bus_dir;

        // set pch and pcl
        char *pc_str = bin_to_str(pc_reg.bin_value);

        char next_pch_reg_str[PCH_REG_SIZE_BIT];
        char next_pcl_reg_str[PCL_REG_SIZE_BIT];

        strncpy(next_pch_reg_str, pc_str, PCH_REG_SIZE_BIT);
        strncpy(next_pcl_reg_str, pc_str, PCL_REG_SIZE_BIT);

        free(pc_str);

        pch_reg.bin_value = str_to_bin(next_pch_reg_str);
        pcl_reg.bin_value = str_to_bin(next_pcl_reg_str);
    }

    publish_message_to(PC_OUTPUT_BUS_TOPIC, (void *) pc_reg.bin_value);
}
