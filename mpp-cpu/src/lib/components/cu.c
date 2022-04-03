/*
 * File: /src/lib/components/cu.c
 * Project: mpp-cpu
 * File Created: Saturday, 2nd April 2022 1:22:58 pm
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Saturday, 2nd April 2022 1:23:13 pm
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */

#include "cu.h"

#include <stdio.h>
#include <stdlib.h>

#include "../constants.h"
#include "../error.h"
#include "../logger.h"
#include "../pubsub.h"
#include "../utils.h"
#include "acumm.h"
#include "addsub.h"
#include "alu.h"
#include "components.h"
#include "controllers/flagsinta.h"
#include "controllers/mxfldx.h"
#include "fffc.h"
#include "flags.h"
#include "greg.h"
#include "hl.h"
#include "mem.h"
#include "mxdir.h"
#include "mxreg.h"
#include "op2.h"
#include "pc.h"
#include "sp.h"

Register RI_reg = {.bin_value = 00000000, .bit_length = RI_REG_SIZE_BIT};
LoadBit ricar_lb = {.value = 0};

static DataBus_t last_bus_data;
static FlagsOutputBus_t last_bus_flags_out;
static PubSubSubscription *data_bus_topic_subscription = NULL;
static PubSubSubscription *flags_out_bus_topic_subscription = NULL;

static void on_bus_data_message(PubSubMessage m) { last_bus_data = *(DataBus_t *)m.value; }
static void on_bus_flags_out_message(PubSubMessage m) { last_bus_flags_out = *(FlagsOutputBus_t *)m.value; }

static void set_ricar_lb(void) { ricar_lb.value = 1; }
static void reset_ricar_lb(void) { ricar_lb.value = 0; }

static OpStateTrace *create_state_trace(void) {
    OpStateTrace *trace = malloc(sizeof(OpStateTrace));
    if (trace == NULL) {
        Error err = {.show_errno = false, .type = FATAL_ERROR, .message = "Failed to allocate memory for state trace"};
        throw_error(err);
        return NULL;
    }

    trace->next = NULL;
    return trace;
}
static OpStateTrace *state_trace;

const OpState s0 = {.id = S0,
                    .load_bits = {
                        .sel_alu = -1,
                        .mx_reg = -1,
                        .sel_dir = 00,
                        // .i_d = -1, // <== documentation says this
                        .i_d = 10,  // <== fix
                        .l_e = 1,
                        .reg_bus = 0,
                        .mem_bus = 1,
                        .fl_bus = 0,
                        .ac_bus = 0,
                        .alu_bus = 0,
                        .pch_bus = 0,
                        .pcl_bus = 0,
                        .ri_car = 1,
                        .pc_car = 1,
                        .reg_car = 0,
                        .ac_car = 0,
                        .op2_car = 0,
                        .sel_fl = -1,
                        .fl_car = 0,
                        .h_car = 0,
                        .l_car = 0,
                        .pch_car = 0,
                        .pcl_car = 0,
                        .sp_car = 0,
                    }};
const OpState s1 = {.id = S1,
                    .load_bits = {
                        .sel_alu = -1,
                        .mx_reg = -1,
                        .sel_dir = -1,
                        .i_d = -1,
                        .l_e = 1,
                        .reg_bus = 0,
                        .mem_bus = 0,
                        .fl_bus = 0,
                        .ac_bus = 0,
                        .alu_bus = 0,
                        .pch_bus = 0,
                        .pcl_bus = 0,
                        .ri_car = 0,
                        .pc_car = 0,
                        .reg_car = 0,
                        .ac_car = 0,
                        .op2_car = 0,
                        .sel_fl = -1,
                        .fl_car = 0,
                        .h_car = 0,
                        .l_car = 0,
                        .pch_car = 0,
                        .pcl_car = 0,
                        .sp_car = 0,
                    }};
// TODO all the other OpState
const OpState s2;
const OpState s3;
const OpState s4;
const OpState s5;
const OpState s6;
const OpState s7;
const OpState s8;
const OpState s9;
const OpState s10;
const OpState s11;
const OpState s12;
const OpState s13;
const OpState s14;
const OpState s15;
const OpState s16;
const OpState s17;
const OpState s18;
const OpState s19;
const OpState s20;
const OpState s21;
const OpState s22;
const OpState s23;
const OpState s24;
const OpState s25;
const OpState s26;
const OpState s27;
const OpState s28;
const OpState s29;
const OpState s30;
const OpState s31;
const OpState s32;
const OpState s33;
const OpState s34;
const OpState s35;
const OpState s36;
const OpState s37;
const OpState s38;

void process_state_loadbits(OpState state) {
    if (state.load_bits.sel_alu != -1) set_selalu_lb(state.load_bits.sel_alu);
    if (state.load_bits.mx_reg != -1) set_selreg_lb(state.load_bits.mx_reg);
    if (state.load_bits.sel_dir != -1) set_seldir_lb(state.load_bits.sel_dir);
    if (state.load_bits.i_d != -1) set_id_lb(state.load_bits.i_d);

    switch (state.load_bits.l_e) {
        case 0:
            reset_l_e_lb();
            break;
        case 1:
            set_l_e_lb();
            break;
    };
    switch (state.load_bits.reg_bus) {
        case 0:
            reset_gregbus_lb();
            break;
        case 1:
            set_gregbus_lb();
            break;
    };
    switch (state.load_bits.mem_bus) {
        case 0:
            reset_mem_bus_lb();
            break;
        case 1:
            set_mem_bus_lb();
            break;
    };
    switch (state.load_bits.fl_bus) {
        case 0:
            cll_reset_flbus_lb();
            break;
        case 1:
            cll_set_flbus_lb();
            break;
    };
    switch (state.load_bits.ac_bus) {
        case 0:
            reset_acbus_lb();
            break;
        case 1:
            set_acbus_lb();
            break;
    };
    switch (state.load_bits.alu_bus) {
        case 0:
            reset_alubus_lb();
            break;
        case 1:
            set_alubus_lb();
            break;
    };
    switch (state.load_bits.pch_bus) {
        case 0:
            reset_pchbus_lb();
            break;
        case 1:
            set_pchbus_lb();
            break;
    };
    switch (state.load_bits.pcl_bus) {
        case 0:
            reset_pclbus_lb();
            break;
        case 1:
            set_pclbus_lb();
            break;
    };
    switch (state.load_bits.ri_car) {
        case 0:
            reset_ricar_lb();
            break;
        case 1:
            set_ricar_lb();
            break;
    };
    switch (state.load_bits.pc_car) {
        case 0:
            reset_pccar_lb();
            break;
        case 1:
            set_pccar_lb();
            break;
    };
    switch (state.load_bits.reg_car) {
        case 0:
            reset_gregcar_lb();
            break;
        case 1:
            set_gregcar_lb();
            break;
    };
    switch (state.load_bits.ac_car) {
        case 0:
            reset_accar_lb();
            break;
        case 1:
            set_accar_lb();
            break;
    };
    switch (state.load_bits.op2_car) {
        case 0:
            reset_car2_lb();
            break;
        case 1:
            set_car2_lb();
            break;
    };
    switch (state.load_bits.sel_fl) {
        case 0:
            cll_reset_selfl_lb();
            break;
        case 1:
            cll_set_selfl_lb();
            break;
    };
    switch (state.load_bits.fl_car) {
        case 0:
            reset_flcar_lb();
            break;
        case 1:
            set_flcar_lb();
            break;
    };
    switch (state.load_bits.h_car) {
        case 0:
            reset_hcar_lb();
            break;
        case 1:
            set_hcar_lb();
            break;
    };
    switch (state.load_bits.l_car) {
        case 0:
            reset_lcar_lb();
            break;
        case 1:
            set_lcar_lb();
            break;
    };
    switch (state.load_bits.pch_car) {
        case 0:
            reset_pchcar_lb();
            break;
        case 1:
            set_pchcar_lb();
            break;
    };
    switch (state.load_bits.pcl_car) {
        case 0:
            reset_pclcar_lb();
            break;
        case 1:
            set_pclcar_lb();
            break;
    };
    switch (state.load_bits.sp_car) {
        case 0:
            reset_spcar_lb();
            break;
        case 1:
            set_spcar_lb();
            break;
    };
}

typedef struct {
    int fc;
    int fz;
} FlagsState;

FlagsState get_flags(void) {
    FlagsState flags;
    char *flags_out_str = bin_to_str(last_bus_flags_out);
    flags.fc = flags_out_str[0] - '0';
    flags.fz = flags_out_str[1] - '0';
    free(flags_out_str);
    return flags;
}

OpStateTrace *decode_step(void) {
    OpStateTrace *trace = create_state_trace();
    FlagsState flags = get_flags();

    const int opcode = bin_to_int(RI_reg.bin_value);
    switch (opcode) {
        case 0x00 ... 0x0F: {
            trace->state = s2;
            trace->next = create_state_trace();
            trace->next->state = s3;
            trace->next->next = NULL;
            break;
        }

        case 0x10 ... 0x13: {
            trace->state = s2;
            trace->next = create_state_trace();
            trace->next->state = s4;
            trace->next->next = NULL;
            break;
        }

        case 0x60 ... 0x63: {
            trace->state = s15;
            trace->next = create_state_trace();
            trace->next->state = s3;
            trace->next->next = NULL;
            break;
        }

        case 0x40 ... 0x43: {
            trace->state = s12;
            trace->next = create_state_trace();
            trace->next->state = s3;
            trace->next->next = NULL;
            break;
        }

        case 0x44: {
            trace->state = s12;
            trace->next = create_state_trace();
            trace->next->state = s4;
            trace->next->next = NULL;
            break;
        }

        case 0x64: {
            trace->state = s15;
            trace->next = create_state_trace();
            trace->next->state = s4;
            trace->next->next = NULL;
            break;
        }

        case 0x30 ... 0x33: {
            trace->state = s2;
            trace->next = create_state_trace();
            trace->next->state = s5;
            trace->next->next = NULL;
            break;
        }

        case 0x45: {
            trace->state = s12;
            trace->next = create_state_trace();
            trace->next->state = s5;
            trace->next->next = NULL;
            break;
        }

        case 0x65: {
            trace->state = s15;
            trace->next = create_state_trace();
            trace->next->state = s5;
            trace->next->next = NULL;
            break;
        }

        case 0x18 ... 0x1B: {
            trace->state = s2;
            trace->next = create_state_trace();
            trace->next->state = s6;
            trace->next->next = NULL;
            break;
        }

        case 0x46: {
            trace->state = s12;
            trace->next = create_state_trace();
            trace->next->state = s6;
            trace->next->next = NULL;
            break;
        }

        case 0x66: {
            trace->state = s15;
            trace->next = create_state_trace();
            trace->next->state = s6;
            trace->next->next = NULL;
            break;
        }

        case 0x1C ... 0x1F: {
            trace->state = s2;
            trace->next = create_state_trace();
            trace->next->state = s7;
            trace->next->next = NULL;
            break;
        }

        case 0x47: {
            trace->state = s12;
            trace->next = create_state_trace();
            trace->next->state = s7;
            trace->next->next = NULL;
            break;
        }

        case 0x67: {
            trace->state = s15;
            trace->next = create_state_trace();
            trace->next->state = s7;
            trace->next->next = NULL;
            break;
        }

        case 0x2C ... 0x2F: {
            trace->state = s2;
            trace->next = create_state_trace();
            trace->next->state = s8;
            trace->next->next = NULL;
            break;
        }

        case 0x4B: {
            trace->state = s12;
            trace->next = create_state_trace();
            trace->next->state = s13;
            trace->next->next = NULL;
            break;
        }

        case 0x20 ... 0x23: {
            trace->state = s2;
            trace->next = create_state_trace();
            trace->next->state = s9;
            trace->next->next = NULL;
            break;
        }

        case 0x48: {
            trace->state = s12;
            trace->next = create_state_trace();
            trace->next->state = s9;
            trace->next->next = NULL;
            break;
        }

        case 0x68: {
            trace->state = s15;
            trace->next = create_state_trace();
            trace->next->state = s9;
            trace->next->next = NULL;
            break;
        }

        case 0x24 ... 0x27: {
            trace->state = s2;
            trace->next = create_state_trace();
            trace->next->state = s10;
            trace->next->next = NULL;
            break;
        }

        case 0x49: {
            trace->state = s12;
            trace->next = create_state_trace();
            trace->next->state = s10;
            trace->next->next = NULL;
            break;
        }

        case 0x69: {
            trace->state = s15;
            trace->next = create_state_trace();
            trace->next->state = s10;
            trace->next->next = NULL;
            break;
        }

        case 0x28 ... 0x2B: {
            trace->state = s2;
            trace->next = create_state_trace();
            trace->next->state = s11;
            trace->next->next = NULL;
            break;
        }

        case 0x4A: {
            trace->state = s12;
            trace->next = create_state_trace();
            trace->next->state = s11;
            trace->next->next = NULL;
            break;
        }

        case 0x6A: {
            trace->state = s15;
            trace->next = create_state_trace();
            trace->next->state = s11;
            trace->next->next = NULL;
            break;
        }

        case 0x70: {
            trace->state = s16;
            trace->next = create_state_trace();
            trace->next->state = s17;
            trace->next->next = create_state_trace();
            trace->next->next->state = s18;
            trace->next->next->next = create_state_trace();
            trace->next->next->next->state = s4;
            trace->next->next->next->next = NULL;
            break;
        }

        case 0x71: {
            trace->state = s16;
            trace->next = create_state_trace();
            trace->next->state = s17;
            trace->next->next = create_state_trace();
            trace->next->next->state = s12;
            trace->next->next->next = create_state_trace();
            trace->next->next->next->state = s19;
            trace->next->next->next->next = NULL;
            break;
        }

        case 0x72: {
            if (flags.fz) {
                trace->state = s16;
                trace->next = create_state_trace();
                trace->next->state = s17;
                trace->next->next = create_state_trace();
                trace->next->next->state = s20;
                trace->next->next->next = create_state_trace();
                trace->next->next->next->state = s1;
                trace->next->next->next->next = NULL;
            } else {
                trace->state = s16;
                trace->next = create_state_trace();
                trace->next->state = s17;
                trace->next->next = create_state_trace();
                trace->next->next->state = s0;
                trace->next->next->next = NULL;
            }
            break;
        }

        case 0xB0: {
            trace->state = s21;
            trace->next = create_state_trace();
            trace->next->state = s22;
            trace->next->next = create_state_trace();
            trace->next->next->state = s18;
            trace->next->next->next = create_state_trace();
            trace->next->next->next->state = s4;
            trace->next->next->next->next = NULL;
            break;
        }

        case 0x90: {
            trace->state = s16;
            trace->next = create_state_trace();
            trace->next->state = s17;
            trace->next->next = create_state_trace();
            trace->next->next->state = s12;
            trace->next->next->next = create_state_trace();
            trace->next->next->next->state = s19;
            trace->next->next->next->next = NULL;
            break;
        }

        case 0x73: {
            if (flags.fc) {
                trace->state = s16;
                trace->next = create_state_trace();
                trace->next->state = s17;
                trace->next->next = create_state_trace();
                trace->next->next->state = s20;
                trace->next->next->next = create_state_trace();
                trace->next->next->next->state = s1;
                trace->next->next->next->next = NULL;
            } else {
                trace->state = s16;
                trace->next = create_state_trace();
                trace->next->state = s17;
                trace->next->next = create_state_trace();
                trace->next->next->state = s0;
                trace->next->next->next = NULL;
            }
            break;
        }

        case 0x74: {
            trace->state = s16;
            trace->next = create_state_trace();
            trace->next->state = s17;
            trace->next->next = create_state_trace();
            trace->next->next->state = s20;
            trace->next->next->next = create_state_trace();
            trace->next->next->next->state = s1;
            trace->next->next->next->next = NULL;
            break;
        }

        case 0x80: {
            trace->state = s14;
            trace->next = NULL;
            break;
        }

        case 0x81: {
            trace->state = s23;
            trace->next = NULL;
            break;
        }

        case 0x82: {
            trace->state = s24;
            trace->next = NULL;
            break;
        }

        case 0xC1: {
            trace->state = s26;
            trace->next = create_state_trace();
            trace->next->state = s36;
            trace->next->next = NULL;
            break;
        }

        case 0xC2: {
            trace->state = s37;
            trace->next = create_state_trace();
            trace->next->state = s31;
            trace->next->next = NULL;
            break;
        }

        case 0xC3: {
            trace->state = s16;
            trace->next = create_state_trace();
            trace->next->state = s17;
            trace->next->next = create_state_trace();
            trace->next->next->state = s38;
            trace->next->next->next = NULL;
            break;
        }

        case 0xC4: {
            trace->state = s16;
            trace->next = create_state_trace();
            trace->next->state = s17;
            trace->next->next = create_state_trace();
            trace->next->next->state = s26;
            trace->next->next->next = create_state_trace();
            trace->next->next->next->state = s28;
            trace->next->next->next->next = create_state_trace();
            trace->next->next->next->next->state = s26;
            trace->next->next->next->next->next = create_state_trace();
            trace->next->next->next->next->next->state = s29;
            trace->next->next->next->next->next->next = create_state_trace();
            trace->next->next->next->next->next->next->state = s20;
            trace->next->next->next->next->next->next->next = NULL;
            break;
        }

        case 0xC5: {
            trace->state = s32;
            trace->next = create_state_trace();
            trace->next->state = s31;
            trace->next->next = create_state_trace();
            trace->next->next->state = s33;
            trace->next->next->next = create_state_trace();
            trace->next->next->next->state = s31;
            trace->next->next->next->next = NULL;
            break;
        }

        case 0xC0: {
            trace->state = s32;
            trace->next = create_state_trace();
            trace->next->state = s31;
            trace->next->next = create_state_trace();
            trace->next->next->state = s33;
            trace->next->next->next = create_state_trace();
            trace->next->next->next->state = s31;
            trace->next->next->next->next = create_state_trace();
            trace->next->next->next->next->state = s34;
            trace->next->next->next->next->next = create_state_trace();
            trace->next->next->next->next->next->state = s31;
            trace->next->next->next->next->next->next = create_state_trace();
            trace->next->next->next->next->next->next->state = s35;
            trace->next->next->next->next->next->next->next = NULL;
            break;
        }
        default: {
            log_warn("Unhandled opcode: 0x%02X", opcode);
            trace->state = s0;
            trace->next = NULL;
            break;
        }
    }
    return trace;
}

void init_cu(void) {
    data_bus_topic_subscription = subscribe_to(DATA_BUS_TOPIC, on_bus_data_message);
    flags_out_bus_topic_subscription = subscribe_to(FLAGS_OUTPUT_BUS_TOPIC, on_bus_flags_out_message);
    state_trace = (OpStateTrace *)create_state_trace();
    state_trace->state = s0;
    state_trace->next = NULL;
}
void shutdown_cu(void) {
    unsubscribe_for(data_bus_topic_subscription);
    unsubscribe_for(flags_out_bus_topic_subscription);
}

void run_cu(void) {  // 1 opstate per run
    if (ricar_lb.value == 1) {
        // load
        if (get_bin_len(last_bus_data) > RI_reg.bit_length) {
            Error err = {.show_errno = false, .type = NOTICE_ERROR, .message = "Overflow attemping to load to RI register"};
            throw_error(err);
        }

        RI_reg.bin_value = last_bus_data;
    }

    switch (state_trace->state.id) {
        case S0:
            // RI <- (PC)
            process_state_loadbits(s0);

            OpStateTrace *next = (OpStateTrace *)create_state_trace();
            next->state = s1;

            state_trace->next = next;
            break;
        case S1:
            // DECODE
            process_state_loadbits(s1);
            state_trace->next = decode_step();
        default:
            break;
    }

    run_mem();

    run_hl();
    run_addsub();
    run_sp();
    run_pc();
    run_fffc();
    run_mxdir();

    // run_addsub();

    run_greg();
    run_op2();

    run_acumm();

    run_alu();

    cll_run_mxfldx();

    run_flags();

    cll_run_flagsinta();

    OpStateTrace *tmp_next_state_trace = state_trace->next;
    free(state_trace);
    state_trace = tmp_next_state_trace;
}