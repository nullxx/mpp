//
//  mxdir.c
//  mpp-cpu
//
//  Created by https://github.com/nullxx on 23/3/22.
//

#include "mxdir.h"

#include <stdio.h>
#include <stdlib.h>

#include "../electronic/multiplexer.h"
#include "../pubsub.h"
#include "../utils.h"
#include "components.h"

LoadBit seldir_lb = {.value = 00};  // 2 bits

PCOutputBus_t last_bus_pc_output;
SPOutputBus_t last_bus_sp_output;
HLOutputBus_t last_bus_hl_output;
FFFCOutputBus_t last_bus_fffc_output;

PubSubSubscription *pc_output_topic_subscription = NULL;
PubSubSubscription *sp_output_topic_subscription = NULL;
PubSubSubscription *hl_output_topic_subscription = NULL;
PubSubSubscription *fffc_output_topic_subscription = NULL;

void set_seldir_lb(void) { seldir_lb.value = 1; }

void reset_seldir_lb(void) { seldir_lb.value = 0; }

void on_bus_pc_output(PubSubMessage m) { last_bus_pc_output = (DataBus_t)m.value; }

void on_bus_sp_output(PubSubMessage m) { last_bus_sp_output = (DataBus_t)m.value; }

void on_bus_hl_output(PubSubMessage m) { last_bus_hl_output = (DataBus_t)m.value; }

void on_bus_fffc_output(PubSubMessage m) { last_bus_fffc_output = (DataBus_t)m.value; }

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
        err.type = FATAL;
        err.show_errno = false;

        return throw_error(err);
    }

    publish_message_to(DIR_BUS_TOPIC, (void *)output);
    return;
}
