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
#include "../utils.h"
#include "components.h"

LoadBit seldir_lb = {.value = 00};  // 2 bits

static PCOutputBus_t last_bus_pc_output;
static SPOutputBus_t last_bus_sp_output;
static HLOutputBus_t last_bus_hl_output;
static FFFCOutputBus_t last_bus_fffc_output;

static PubSubSubscription *pc_output_topic_subscription = NULL;
static PubSubSubscription *sp_output_topic_subscription = NULL;
static PubSubSubscription *hl_output_topic_subscription = NULL;
static PubSubSubscription *fffc_output_topic_subscription = NULL;

bool set_seldir_lb(unsigned int bin) {
    const int bin_len = get_bin_len(bin);
    if (bin_len > SELDIR_LOAD_BIT_SIZE_BITS) {
        return false;
    }

    seldir_lb.value = bin;
    return true;
}

static void on_bus_pc_output(PubSubMessage m) { last_bus_pc_output = *(DataBus_t *)m.value; }

static void on_bus_sp_output(PubSubMessage m) { last_bus_sp_output = *(SPOutputBus_t *)m.value; }

static void on_bus_hl_output(PubSubMessage m) { last_bus_hl_output = *(HLOutputBus_t *)m.value; }

static void on_bus_fffc_output(PubSubMessage m) { last_bus_fffc_output = *(FFFCOutputBus_t *)m.value; }

void init_mxdir(void) {
    pc_output_topic_subscription = subscribe_to(PC_OUTPUT_BUS_TOPIC, on_bus_pc_output);
    sp_output_topic_subscription = subscribe_to(SP_OUTPUT_BUS_TOPIC, on_bus_sp_output);
    hl_output_topic_subscription = subscribe_to(HL_OUTPUT_BUS_TOPIC, on_bus_hl_output);
    fffc_output_topic_subscription = subscribe_to(FFFC_OUTPUT_BUS_TOPIC, on_bus_fffc_output);
}

void shutdown_mxdir(void) {
    unsubscribe_for(pc_output_topic_subscription);
    unsubscribe_for(sp_output_topic_subscription);
    unsubscribe_for(hl_output_topic_subscription);
    unsubscribe_for(fffc_output_topic_subscription);
}

void run_mxdir(void) {
    Error err;
    MXInput output = run_4x1_mx(seldir_lb.value, last_bus_pc_output, last_bus_sp_output, last_bus_hl_output, last_bus_fffc_output);
    if (output == (MXInput)-1) {
        err.message = "Couldn't run multiplexer";
        err.type = NOTICE_ERROR;
        err.show_errno = false;

        return throw_error(err);
    }

    publish_message_to(DIR_BUS_TOPIC_2, &output);
    return;
}
