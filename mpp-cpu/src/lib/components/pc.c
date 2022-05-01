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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../constants.h"
#include "../electronic/bus.h"
#include "../error.h"
#include "../pubsub.h"
#include "../utils.h"
#include "../watcher.h"
#include "components.h"

static Register pch_reg = {.bit_length = PCH_REG_SIZE_BIT};
static Register pcl_reg = {.bit_length = PCL_REG_SIZE_BIT};
static Register pc_reg = {.bit_length = PC_REG_SIZE_BIT};

RegisterWatcher pch_reg_watcher = {.reg = &pch_reg, .type = WATCHER_TYPE_PCH};
RegisterWatcher pcl_reg_watcher = {.reg = &pcl_reg, .type = WATCHER_TYPE_PCL};
RegisterWatcher pc_reg_watcher = {.reg = &pc_reg, .type = WATCHER_TYPE_PC};

static Bus_t *last_bus_dir = NULL;
static Bus_t *last_bus_data = NULL;
static Bus_t *control_bus = NULL;

static PubSubSubscription *dir_bus_topic_subscription = NULL;
static PubSubSubscription *data_bus_topic_subscription = NULL;
static PubSubSubscription *control_bus_topic_subscription = NULL;

void init_pc(void) {
    initialize_word(&pch_reg.value, 0);
    initialize_word(&pcl_reg.value, 0);
    initialize_word(&pc_reg.value, 0);

    last_bus_dir = create_bus_data();
    last_bus_data = create_bus_data();
    control_bus = create_bus_data();
    dir_bus_topic_subscription = subscribe_to(DIR_BUS_TOPIC_1, last_bus_dir);
    data_bus_topic_subscription = subscribe_to(DATA_BUS_TOPIC, last_bus_data);
    control_bus_topic_subscription = subscribe_to(CONTROL_BUS_TOPIC, control_bus);

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
    unsubscribe_for(control_bus_topic_subscription);

    destroy_bus_data(last_bus_dir);
    destroy_bus_data(last_bus_data);
    destroy_bus_data(control_bus);
}

void run_pc(void) {
    update_bus_data(last_bus_data);
    update_bus_data(last_bus_dir);
    update_bus_data(control_bus);

    Word pchcar_lb, pclcar_lb, pch_bus, pcl_bus, pccar_lb;
    initialize_word(&pchcar_lb, 0);
    initialize_word(&pclcar_lb, 0);
    initialize_word(&pch_bus, 0);
    initialize_word(&pcl_bus, 0);
    initialize_word(&pccar_lb, 0);

    pchcar_lb.bits[0] = control_bus->current_value.bits[CONTROL_BUS_PCHCAR_BIT_POSITION];
    pclcar_lb.bits[0] = control_bus->current_value.bits[CONTROL_BUS_PCLCAR_BIT_POSITION];
    pch_bus.bits[0] = control_bus->current_value.bits[CONTROL_BUS_PCHBUS_BIT_POSITION];
    pcl_bus.bits[0] = control_bus->current_value.bits[CONTROL_BUS_PCLBUS_BIT_POSITION];
    pccar_lb.bits[0] = control_bus->current_value.bits[CONTROL_BUS_PCCAR_BIT_POSITION];

    // pch
    if (word_to_int(pchcar_lb) == 1) {
        // load pch
        if (get_used_bits(last_bus_data->current_value) > pch_reg.bit_length) {
            Error err = {.show_errno = 0, .type = NOTICE_ERROR, .message = "Overflow attemping to load PCH register"};
            return throw_error(err);
        }

        pch_reg.value = last_bus_data->current_value;
    } else if (word_to_int(pch_bus) == 1) {  // pchcar_lb.value = 0
        // read to data bus if pchbus enabled
        publish_message_to(DATA_BUS_TOPIC, pch_reg.value);
    }
    // pcl
    if (word_to_int(pclcar_lb) == 1) {
        // load pcl
        if (get_used_bits(last_bus_data->current_value) > pcl_reg.bit_length) {
            Error err = {.show_errno = 0, .type = NOTICE_ERROR, .message = "Overflow attemping to load PCL register"};
            return throw_error(err);
        }

        pcl_reg.value = last_bus_data->current_value;
    } else if (word_to_int(pcl_bus) == 1) {  // pchcar_lb.value = 0
        // read to data bus if pchbus enabled
        publish_message_to(DATA_BUS_TOPIC, pcl_reg.value);
    }

    // mix pch + pcl => pc. If pc is set later it will be overwritten
    if (word_to_int(pchcar_lb) == 1 && word_to_int(pclcar_lb) == 1) {
        // PC = PCH|PCL
        for (int i = 0; i < pcl_reg.bit_length; i++) {
            pc_reg.value.bits[i] = pcl_reg.value.bits[i];
        }

        for (int i = 0; i < pch_reg.bit_length; i++) {
            pc_reg.value.bits[pcl_reg.bit_length + i] = pch_reg.value.bits[i];
        }
    }

    // pc
    if (word_to_int(pccar_lb) == 1) {
        // load pc // PC is prioritary than pch and pcl
        int used_bits = get_used_bits(last_bus_dir->current_value);
        if (used_bits > pc_reg.bit_length) {
            Error err = {.show_errno = 0, .type = NOTICE_ERROR, .message = "Overflow attemping to load PC register"};
            return throw_error(err);
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
