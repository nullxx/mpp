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

#include <stdio.h>

#include "../constants.h"
#include "../electronic/multiplexer.h"
#include "../pubsub.h"
#include "../utils.h"
#include "components.h"

LoadBit selreg_lb = {.value = 00};

bool set_selreg_lb(unsigned int sel) {
    const int num_len = get_bin_len(sel);
    if (num_len != SELREG_LOAD_BIT_SIZE_BITS) {
        return false;
    }

    selreg_lb.value = sel;
    return true;
}

void init_mxreg(void) {}
void shutdown_mxreg(void) {}
void run_mxreg(void) {
    SelRegOutputBus_t result = (SelRegOutputBus_t)run_4x1_mx(selreg_lb.value, 00, 01, 10, 11);
    publish_message_to(SELREG_OUTPUT_BUS_TOPIC, (void *)(intptr_t)result);
}
