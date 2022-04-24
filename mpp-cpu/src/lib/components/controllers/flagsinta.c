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
#include "../../electronic/bus.h"
#include "../../error.h"
#include "../../pubsub.h"
#include "../../utils.h"
#include "../components.h"

static Bus_t *last_bus_flags_out = NULL;
static Bus_t *control_bus = NULL;

static PubSubSubscription *flags_out_bus_topic_subscription = NULL;
static PubSubSubscription *control_bus_topic_subscription = NULL;

void cll_init_flagsinta(void) {
    last_bus_flags_out = create_bus_data();
    control_bus = create_bus_data();
    flags_out_bus_topic_subscription = subscribe_to(FLAGS_OUTPUT_BUS_TOPIC, last_bus_flags_out);
    control_bus_topic_subscription = subscribe_to(CONTROL_BUS_TOPIC, control_bus);
}

void cll_shutdown_flagsinta(void) {
    unsubscribe_for(flags_out_bus_topic_subscription);
    unsubscribe_for(control_bus_topic_subscription);

    destroy_bus_data(last_bus_flags_out);
    destroy_bus_data(control_bus);
}

void cll_run_flagsinta(void) {
    update_bus_data(last_bus_flags_out);
    update_bus_data(control_bus);

    Word flbus_lb;
    initialize_word(&flbus_lb, 0);

    flbus_lb.bits[0] = control_bus->current_value.bits[CONTROL_BUS_FLBUS_BIT_POSITION];

    Word to_send;
    initialize_word(&to_send, 0);

    // FC|INTA|X|X|X|X|X|FZ
    to_send.bits[FLAGSINTA_FC_BUS_BIT_POSITION] = last_bus_flags_out->current_value.bits[FLAGSINTA_FC_BUS_BIT_POSITION];
    to_send.bits[FLAGSINTA_FZ_BUS_BIT_POSITION] = last_bus_flags_out->current_value.bits[FLAGSINTA_FZ_BUS_BIT_POSITION];
    // rest of the bits are 0
    to_send.bits[FLAGSINTA_INTA_BUS_BIT_POSITION] = 0;  // TODO: Implement INTA

    if (word_to_int(flbus_lb)) {
        // load
        publish_message_to(DATA_BUS_TOPIC, to_send);
    }
}