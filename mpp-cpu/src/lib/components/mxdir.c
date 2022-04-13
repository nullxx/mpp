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
#include "../electronic/multiplexer.h"
#include "../error.h"
#include "../pubsub.h"
#include "../thread.h"
#include "../utils.h"
#include "components.h"
#include "../electronic/bus.h"

LoadBit seldir_lb = {.value = 00};  // 2 bits

static Bus_t last_bus_pc_output = {.current_value = 0, .next_value = 0};
static Bus_t last_bus_sp_output = {.current_value = 0, .next_value = 0};
static Bus_t last_bus_hl_output = {.current_value = 0, .next_value = 0};
static Bus_t last_bus_fffc_output = {.current_value = 0, .next_value = 0};

static PubSubSubscription *pc_output_topic_subscription = NULL;
static PubSubSubscription *sp_output_topic_subscription = NULL;
static PubSubSubscription *hl_output_topic_subscription = NULL;
static PubSubSubscription *fffc_output_topic_subscription = NULL;

bool set_seldir_lb(unsigned int bin) {
    const int bin_len = get_num_len(bin);
    if (bin_len > SELDIR_LOAD_BIT_SIZE_BITS) {
        return false;
    }

    seldir_lb.value = bin;
    return true;
}

void init_mxdir(void) {
    pc_output_topic_subscription = subscribe_to(PC_OUTPUT_BUS_TOPIC, &last_bus_pc_output);
    sp_output_topic_subscription = subscribe_to(SP_OUTPUT_BUS_TOPIC, &last_bus_sp_output);
    hl_output_topic_subscription = subscribe_to(HL_OUTPUT_BUS_TOPIC, &last_bus_hl_output);
    fffc_output_topic_subscription = subscribe_to(FFFC_OUTPUT_BUS_TOPIC, &last_bus_fffc_output);
}

void shutdown_mxdir(void) {
    unsubscribe_for(pc_output_topic_subscription);
    unsubscribe_for(sp_output_topic_subscription);
    unsubscribe_for(hl_output_topic_subscription);
    unsubscribe_for(fffc_output_topic_subscription);
}

void run_mxdir(void) {
    update_bus_data(&last_bus_pc_output);
    update_bus_data(&last_bus_sp_output);
    update_bus_data(&last_bus_hl_output);
    update_bus_data(&last_bus_fffc_output);

    Error err;
    MXInput output = run_4x1_mx(seldir_lb.value, last_bus_pc_output.next_value, last_bus_sp_output.next_value, last_bus_hl_output.next_value, last_bus_fffc_output.next_value);
    if (output == (MXInput)-1) {
        err.message = "Couldn't run multiplexer";
        err.type = NOTICE_ERROR;
        err.show_errno = false;

        return throw_error(err);
    }

    publish_message_to(DIR_BUS_TOPIC_2, output);
    return;
}
