//
//  mxdir.c
//  mpp-cpu
//
//  Created by https://github.com/nullxx on 23/3/22.
//

#include "mxdir.h"

#include <stdio.h>
#include <stdlib.h>

#include "../constants.h"
#include "../electronic/bus.h"
#include "../electronic/multiplexer.h"
#include "../error.h"
#include "../pubsub.h"
#include "../thread.h"
#include "../utils.h"
#include "components.h"

static Bus_t *last_bus_pc_output = NULL;
static Bus_t *last_bus_sp_output = NULL;
static Bus_t *last_bus_hl_output = NULL;
static Bus_t *last_bus_fffc_output = NULL;
static Bus_t *control_bus = NULL;

static PubSubSubscription *pc_output_topic_subscription = NULL;
static PubSubSubscription *sp_output_topic_subscription = NULL;
static PubSubSubscription *hl_output_topic_subscription = NULL;
static PubSubSubscription *fffc_output_topic_subscription = NULL;
static PubSubSubscription *control_bus_topic_subscription = NULL;

void init_mxdir(void) {
    last_bus_pc_output = create_bus_data();
    last_bus_sp_output = create_bus_data();
    last_bus_hl_output = create_bus_data();
    last_bus_fffc_output = create_bus_data();
    control_bus = create_bus_data();
    pc_output_topic_subscription = subscribe_to(PC_OUTPUT_BUS_TOPIC, last_bus_pc_output);
    sp_output_topic_subscription = subscribe_to(SP_OUTPUT_BUS_TOPIC, last_bus_sp_output);
    hl_output_topic_subscription = subscribe_to(HL_OUTPUT_BUS_TOPIC, last_bus_hl_output);
    fffc_output_topic_subscription = subscribe_to(FFFC_OUTPUT_BUS_TOPIC, last_bus_fffc_output);
    control_bus_topic_subscription = subscribe_to(CONTROL_BUS_TOPIC, control_bus);
}

void shutdown_mxdir(void) {
    unsubscribe_for(pc_output_topic_subscription);
    unsubscribe_for(sp_output_topic_subscription);
    unsubscribe_for(hl_output_topic_subscription);
    unsubscribe_for(fffc_output_topic_subscription);
    unsubscribe_for(control_bus_topic_subscription);

    destroy_bus_data(last_bus_pc_output);
    destroy_bus_data(last_bus_sp_output);
    destroy_bus_data(last_bus_hl_output);
    destroy_bus_data(last_bus_fffc_output);
    destroy_bus_data(control_bus);
}

void run_mxdir(void) {
    update_bus_data(last_bus_pc_output);
    update_bus_data(last_bus_sp_output);
    update_bus_data(last_bus_hl_output);
    update_bus_data(last_bus_fffc_output);
    update_bus_data(control_bus);

    Word seldir_lb;
    initialize_word(&seldir_lb, 0);

    seldir_lb.bits[0] = control_bus->current_value.bits[CONTROL_BUS_SELDIR_0_BIT_POSITION];
    seldir_lb.bits[1] = control_bus->current_value.bits[CONTROL_BUS_SELDIR_1_BIT_POSITION];

    Error err;
    MXInput output = run_4x1_mx(word_to_int(seldir_lb), word_to_int(last_bus_pc_output->next_value), word_to_int(last_bus_sp_output->next_value), word_to_int(last_bus_hl_output->next_value),
                                word_to_int(last_bus_fffc_output->next_value));
    if (output == (MXInput)-1) {
        err.message = "Couldn't run multiplexer";
        err.type = NOTICE_ERROR;
        err.show_errno = false;

        return throw_error(err);
    }

    publish_message_to(DIR_BUS_TOPIC_2, int_to_word(output));
    return;
}
