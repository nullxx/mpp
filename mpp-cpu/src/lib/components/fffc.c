/*
 * File: /src/lib/components/fffc.c
 * Project: mpp-cpu
 * File Created: Tuesday, 29th March 2022 10:11:29 pm
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Tuesday, 29th March 2022 10:12:40 pm
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */

#include "fffc.h"

#include "../pubsub.h"
#include "../utils.h"
#include "../constants.h"

void init_fffc(void) {}
void shutdown_fffc(void) {}
void run_fffc(void) {
    unsigned long long bin = int_to_bin(hex_to_int("FFFC"), MAX_CALC_BIN_SIZE_BITS);
    publish_message_to(FFFC_OUTPUT_BUS_TOPIC, &bin);
}