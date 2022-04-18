/*
 * File: /src/lib/components/mxreg.c
 * Project: mpp-cpu
 * File Created: Wednesday, 30th March 2022 11:42:13 am
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Wednesday, 30th March 2022 11:42:30 am
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */

#include "mxreg.h"

#include <stdint.h>
#include <stdio.h>

#include "../constants.h"
#include "../electronic/multiplexer.h"
#include "../pubsub.h"
#include "../utils.h"
#include "components.h"

LoadBit selreg_lb = {.value = 00};

static Bus_t *cu_mreg_jnr_bus_1 = NULL;
static Bus_t *cu_mreg_jnr_bus_2 = NULL;
static PubSubSubscription *last_cu_mxreg_jnr_output_1_subscription = NULL;
static PubSubSubscription *last_cu_mxreg_jnr_output_2_subscription = NULL;

bool set_selreg_lb(unsigned int sel) {
    const int num_len = get_used_bits(sel);
    if (num_len > SELREG_LOAD_BIT_SIZE_BITS) {
        return false;
    }

    selreg_lb.value = sel;
    return true;
}

void init_mxreg(void) {
    cu_mreg_jnr_bus_1 = create_bus_data();
    last_cu_mxreg_jnr_output_1_subscription = subscribe_to(CU_MXREG_JNR_OUTPUT_1_BUS_TOPIC, cu_mreg_jnr_bus_1);
    cu_mreg_jnr_bus_2 = create_bus_data();
    last_cu_mxreg_jnr_output_2_subscription = subscribe_to(CU_MXREG_JNR_OUTPUT_2_BUS_TOPIC, cu_mreg_jnr_bus_2);
}

void shutdown_mxreg(void) {
    unsubscribe_for(last_cu_mxreg_jnr_output_1_subscription);
    unsubscribe_for(last_cu_mxreg_jnr_output_2_subscription);

    destroy_bus_data(cu_mreg_jnr_bus_1);
    destroy_bus_data(cu_mreg_jnr_bus_2);
}

void run_mxreg(void) {
    update_bus_data(cu_mreg_jnr_bus_1);
    update_bus_data(cu_mreg_jnr_bus_2);

    int result = run_4x1_mx(selreg_lb.value, word_to_int(cu_mreg_jnr_bus_1->current_value), word_to_int(cu_mreg_jnr_bus_2->current_value), 0b10, 0b11);
    publish_message_to(SELREG_OUTPUT_BUS_TOPIC, int_to_word(result));
}
