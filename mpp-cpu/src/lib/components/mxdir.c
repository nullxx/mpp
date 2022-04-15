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

LoadBit seldir_lb = {.value = 00};  // 2 bits

static Bus_t *last_bus_pc_output = NULL;
static Bus_t *last_bus_sp_output = NULL;
static Bus_t *last_bus_hl_output = NULL;
static Bus_t *last_bus_fffc_output = NULL;

static PubSubSubscription *pc_output_topic_subscription = NULL;
static PubSubSubscription *sp_output_topic_subscription = NULL;
static PubSubSubscription *hl_output_topic_subscription = NULL;
static PubSubSubscription *fffc_output_topic_subscription = NULL;

bool set_seldir_lb(unsigned int bin) {
    const int bin_len = get_used_bits(bin);
    if (bin_len > SELDIR_LOAD_BIT_SIZE_BITS) {
        return false;
    }

    seldir_lb.value = bin;
    return true;
}

void init_mxdir(void) {
    last_bus_pc_output = create_bus_data();
    last_bus_sp_output = create_bus_data();
    last_bus_hl_output = create_bus_data();
    last_bus_fffc_output = create_bus_data();
    pc_output_topic_subscription = subscribe_to(PC_OUTPUT_BUS_TOPIC, last_bus_pc_output);
    sp_output_topic_subscription = subscribe_to(SP_OUTPUT_BUS_TOPIC, last_bus_sp_output);
    hl_output_topic_subscription = subscribe_to(HL_OUTPUT_BUS_TOPIC, last_bus_hl_output);
    fffc_output_topic_subscription = subscribe_to(FFFC_OUTPUT_BUS_TOPIC, last_bus_fffc_output);
}

void shutdown_mxdir(void) {
    unsubscribe_for(pc_output_topic_subscription);
    unsubscribe_for(sp_output_topic_subscription);
    unsubscribe_for(hl_output_topic_subscription);
    unsubscribe_for(fffc_output_topic_subscription);

    destroy_bus_data(last_bus_pc_output);
    destroy_bus_data(last_bus_sp_output);
    destroy_bus_data(last_bus_hl_output);
    destroy_bus_data(last_bus_fffc_output);
}

void run_mxdir(void) {
    update_bus_data(last_bus_pc_output);
    update_bus_data(last_bus_sp_output);
    update_bus_data(last_bus_hl_output);
    update_bus_data(last_bus_fffc_output);

    Error err;
    MXInput output = run_4x1_mx(seldir_lb.value, word_to_int(last_bus_pc_output->next_value), word_to_int(last_bus_sp_output->next_value), word_to_int(last_bus_hl_output->next_value), word_to_int(last_bus_fffc_output->next_value));
    if (output == (MXInput)-1) {
        err.message = "Couldn't run multiplexer";
        err.type = NOTICE_ERROR;
        err.show_errno = false;

        return throw_error(err);
    }

    publish_message_to(DIR_BUS_TOPIC_2, int_to_word(output));
    return;
}
