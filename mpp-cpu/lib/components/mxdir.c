//
//  mxdir.c
//  mpp-cpu
//
//  Created by https://github.com/nullxx on 23/3/22.
//


#include "mxdir.h"
#include "components.h"
#include "../pubsub.h"
#include "../electronic/multiplexer.h"
#include "../utils.h"
#include <stdio.h>


LoadBit seldir_lb = {.value = 00}; // 2 bits

PCOutputBus_t last_bus_pc_output;
SPOutputBus_t last_bus_sp_output;
HLOutputBus_t last_bus_hl_output;
FFFCOutputBus_t last_bus_fffc_output;

void set_seldir_lb(void) {
	seldir_lb.value = 1;
}

void reset_seldir_lb(void) {
	seldir_lb.value = 0;
}

void on_bus_pc_output(PubSubMessage m) {
	last_bus_pc_output = (DataBus_t) m.value;
}

void on_bus_sp_output(PubSubMessage m) {
	last_bus_sp_output = (DataBus_t) m.value;
}

void on_bus_hl_output(PubSubMessage m) {
	last_bus_hl_output = (DataBus_t) m.value;
}

void on_bus_fffc_output(PubSubMessage m) {
	last_bus_fffc_output = (DataBus_t) m.value;
}

void init_mxdir(void) {
	subscribe_to(PC_OUTPUT_BUS_TOPIC, on_bus_pc_output);
	subscribe_to(SP_OUTPUT_BUS_TOPIC, on_bus_sp_output);
	subscribe_to(HL_OUTPUT_BUS_TOPIC, on_bus_hl_output);
	subscribe_to(FFFC_OUTPUT_BUS_TOPIC, on_bus_fffc_output);
}

void run_mxdir(void) {
	MXInput output = run_4x1_mx(seldir_lb.value, last_bus_pc_output, last_bus_sp_output, last_bus_hl_output, last_bus_fffc_output);
    if (output == -1) {
        Error err = {
            .message = "Couldn't run multiplexer",
            .show_errno = false,
            .type = FATAL
        };

        return process_error(&err);
    }

	publish_message_to(DIR_BUS_TOPIC, bin_to_str(output));
}
