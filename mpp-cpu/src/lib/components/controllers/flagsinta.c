/*
 * File: /src/lib/components/controllers/flagsinta.c
 * Project: mpp-cpu
 * File Created: Saturday, 2nd April 2022 12:34:43 pm
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Saturday, 2nd April 2022 12:34:54 pm
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */
#include "flagsinta.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../constants.h"
#include "../../error.h"
#include "../../pubsub.h"
#include "../../utils.h"
#include "../components.h"

LoadBit flbus_lb = {.value = 0};
static FlagsOutputBus_t last_bus_flags_out;
static PubSubSubscription *flags_out_bus_topic_subscription = NULL;

static void on_bus_flags_out_message(PubSubMessage m) { last_bus_flags_out = *(FlagsOutputBus_t *)m.value; }

void cll_set_flbus_lb(void) { flbus_lb.value = 1; }
void cll_reset_flbus_lb(void) { flbus_lb.value = 0; }

void cll_init_flagsinta(void) { flags_out_bus_topic_subscription = subscribe_to(FLAGS_OUTPUT_BUS_TOPIC, on_bus_flags_out_message); }

void cll_shutdown_flagsinta(void) { unsubscribe_for(flags_out_bus_topic_subscription); }

void cll_run_flagsinta(void) {
    // FC|INTA|X|X|X|X|X|FZ
    char *flags_str = itoa(last_bus_flags_out);
    const int flags_str_len = strlen(flags_str);

    const int bin_str_len = FLAGSINTA_BUS_SIZE_BITS;
    char *bin_str = (char *)malloc(sizeof(char) * (bin_str_len + 1));
    initialize_str(bin_str, 0, FLAGSINTA_BUS_SIZE_BITS, 0);

    if (FLAGSINTA_FC_BUS_BIT_POSITION > bin_str_len || FLAGSINTA_FZ_BUS_BIT_POSITION > bin_str_len || FLAGSINTA_FC_BUS_BIT_POSITION > flags_str_len || FLAGSINTA_FZ_BUS_BIT_POSITION > flags_str_len) {
        free(bin_str);
        free(flags_str);
        return;
    }

    bin_str[FLAGSINTA_FC_BUS_BIT_POSITION] = flags_str[FLAGSINTA_FC_BUS_BIT_POSITION];
    bin_str[FLAGSINTA_FZ_BUS_BIT_POSITION] = flags_str[FLAGSINTA_FZ_BUS_BIT_POSITION];
    initialize_str(bin_str, FLAGSINTA_GND_START_BIT_POSITION, FLAGSINTA_GND_END_BIT_POSITION, FLAGSINTA_GND_CHAR_VALUE);
    bin_str[FLAGSINTA_INTA_BUS_BIT_POSITION] = 0;  // TODO: Implement INTA

    unsigned long long bin = str_to_bin(bin_str);

    free(bin_str);
    free(flags_str);

    if (flbus_lb.value) {
        // load
        publish_message_to(DATA_BUS_TOPIC, &bin);
    }
}