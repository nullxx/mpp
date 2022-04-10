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

#include "../../constants.h"
#include "../../error.h"
#include "../../logger.h"
#include "../../pubsub.h"
#include "../../utils.h"
#include "../../watcher.h"
#include "../acumm.h"
#include "../addsub.h"
#include "../alu.h"
#include "../components.h"
#include "../controllers/flagsinta.h"
#include "../controllers/mxfldx.h"
#include "../fffc.h"
#include "../flags.h"
#include "../greg.h"
#include "../hl.h"
#include "../mem.h"
#include "../mxdir.h"
#include "../mxreg.h"
#include "../op2.h"
#include "../pc.h"
#include "../sp.h"
#include "cu_constants.h"

Register RI_reg = {.bin_value = 00000000, .bit_length = RI_REG_SIZE_BIT};

RegisterWatcher RI_reg_watcher = {.name = "RI", .reg = &RI_reg};

LoadBit ricar_lb = {.value = 0};

static Bus_t last_bus_data;
static Bus_t last_bus_flags_out;
static PubSubSubscription *data_bus_topic_subscription = NULL;
static PubSubSubscription *flags_out_bus_topic_subscription = NULL;

static void set_ricar_lb(void) { ricar_lb.value = 1; }
static void reset_ricar_lb(void) { ricar_lb.value = 0; }

static OpStateTrace *create_state_trace(const OpState *state) {
    OpStateTrace *trace = (OpStateTrace *)malloc(sizeof(OpStateTrace));
    if (trace == NULL) {
        Error err = {.show_errno = false, .type = FATAL_ERROR, .message = "Failed to allocate memory for state trace"};
        throw_error(err);
        return NULL;
    }
    if (state != NULL) trace->state = *state;
    trace->next = NULL;
    return trace;
}

static OpStateTrace *add_next_state_trace_from(OpStateTrace *trace, OpState state) {
    if (trace == NULL) return NULL;

    trace->next = create_state_trace(&state);
    if (trace->next == NULL) return NULL;
    trace->next->state = state;

    return trace->next;
}

static OpStateTrace *state_trace;

void process_loadbit_switch(int activate, void (*set_fn)(void), void (*reset_fn)(void)) {
    if (activate) {
        set_fn();
    } else {
        reset_fn();
    }
}

void process_state_loadbits(OpState state) {
    if (state.load_bits.sel_alu != -1) set_selalu_lb(state.load_bits.sel_alu);
    if (state.load_bits.mx_reg != -1) set_selreg_lb(state.load_bits.mx_reg);
    if (state.load_bits.sel_dir != -1) set_seldir_lb(state.load_bits.sel_dir);

    // state.load_bits.sel_alu != -1 ? set_selalu_lb(state.load_bits.sel_alu) : set_selalu_lb(TRANSPARENT);
    state.load_bits.i_d != -1 ? set_id_lb(state.load_bits.i_d) : set_id_lb(0);

    process_loadbit_switch(state.load_bits.l_e, set_l_e_lb, reset_l_e_lb);
    process_loadbit_switch(state.load_bits.reg_bus, set_gregbus_lb, reset_gregbus_lb);
    process_loadbit_switch(state.load_bits.mem_bus, set_mem_bus_lb, reset_mem_bus_lb);
    process_loadbit_switch(state.load_bits.fl_bus, cll_set_flbus_lb, cll_reset_flbus_lb);
    process_loadbit_switch(state.load_bits.ac_bus, set_acbus_lb, reset_acbus_lb);
    process_loadbit_switch(state.load_bits.alu_bus, set_alubus_lb, reset_alubus_lb);
    process_loadbit_switch(state.load_bits.pch_bus, set_pchbus_lb, reset_pchbus_lb);
    process_loadbit_switch(state.load_bits.pcl_bus, set_pclbus_lb, reset_pclbus_lb);
    process_loadbit_switch(state.load_bits.ri_car, set_ricar_lb, reset_ricar_lb);
    process_loadbit_switch(state.load_bits.pc_car, set_pccar_lb, reset_pccar_lb);
    process_loadbit_switch(state.load_bits.reg_car, set_gregcar_lb, reset_gregcar_lb);
    process_loadbit_switch(state.load_bits.ac_car, set_accar_lb, reset_accar_lb);
    process_loadbit_switch(state.load_bits.op2_car, set_car2_lb, reset_car2_lb);
    process_loadbit_switch(state.load_bits.sel_fl, cll_set_selfl_lb, cll_reset_selfl_lb);
    process_loadbit_switch(state.load_bits.fl_car, set_flcar_lb, reset_flcar_lb);
    process_loadbit_switch(state.load_bits.h_car, set_hcar_lb, reset_hcar_lb);
    process_loadbit_switch(state.load_bits.l_car, set_lcar_lb, reset_lcar_lb);
    process_loadbit_switch(state.load_bits.pch_car, set_pchcar_lb, reset_pchcar_lb);
    process_loadbit_switch(state.load_bits.pcl_car, set_pclcar_lb, reset_pclcar_lb);
    process_loadbit_switch(state.load_bits.sp_car, set_spcar_lb, reset_spcar_lb);
}

typedef struct {
    int fc;
    int fz;
} FlagsState;

FlagsState get_flags(void) {
    FlagsState flags;
    int flags_int = bin_to_int(last_bus_flags_out);
    flags.fc = flags_int == 10;
    flags.fz = flags_int == 1;
    if (flags_int == 11) {
        flags.fc = 1;
        flags.fz = 1;
    }
    return flags;
}

OpStateTrace *decode_step(void) {
    OpStateTrace *trace = create_state_trace(NULL);
    FlagsState flags = get_flags();

    const int opcode = bin_to_int(RI_reg.bin_value);
    log_debug("Decoding opcode: 0x%02X", opcode);
    switch (opcode) {
        case 0x00 ... 0x0F: {
            trace->state = s2;
            add_next_state_trace_from(trace, s3);
            break;
        }

        case 0x10 ... 0x13: {
            trace->state = s2;
            add_next_state_trace_from(trace, s4);
            break;
        }

        case 0x60 ... 0x63: {
            trace->state = s15;
            add_next_state_trace_from(trace, s3);
            break;
        }

        case 0x40 ... 0x43: {
            trace->state = s12;
            add_next_state_trace_from(trace, s3);
            break;
        }

        case 0x44: {
            trace->state = s12;
            add_next_state_trace_from(trace, s4);
            break;
        }

        case 0x64: {
            trace->state = s15;
            add_next_state_trace_from(trace, s4);
            break;
        }

        case 0x30 ... 0x33: {
            trace->state = s2;
            add_next_state_trace_from(trace, s5);
            break;
        }

        case 0x45: {
            trace->state = s12;
            add_next_state_trace_from(trace, s5);
            break;
        }

        case 0x65: {
            trace->state = s15;
            add_next_state_trace_from(trace, s5);
            break;
        }

        case 0x18 ... 0x1B: {
            trace->state = s2;
            add_next_state_trace_from(trace, s6);
            break;
        }

        case 0x46: {
            trace->state = s12;
            add_next_state_trace_from(trace, s6);
            break;
        }

        case 0x66: {
            trace->state = s15;
            add_next_state_trace_from(trace, s6);
            break;
        }

        case 0x1C ... 0x1F: {
            trace->state = s2;
            add_next_state_trace_from(trace, s7);
            break;
        }

        case 0x47: {
            trace->state = s12;
            add_next_state_trace_from(trace, s7);
            break;
        }

        case 0x67: {
            trace->state = s15;
            add_next_state_trace_from(trace, s7);
            break;
        }

        case 0x2C ... 0x2F: {
            trace->state = s2;
            add_next_state_trace_from(trace, s8);
            break;
        }

        case 0x4B: {
            trace->state = s12;
            add_next_state_trace_from(trace, s13);
            break;
        }

        case 0x20 ... 0x23: {
            trace->state = s2;
            add_next_state_trace_from(trace, s9);
            break;
        }

        case 0x48: {
            trace->state = s12;
            add_next_state_trace_from(trace, s9);
            break;
        }

        case 0x68: {
            trace->state = s15;
            add_next_state_trace_from(trace, s9);
            break;
        }

        case 0x24 ... 0x27: {
            trace->state = s2;
            add_next_state_trace_from(trace, s10);
            break;
        }

        case 0x49: {
            trace->state = s12;
            add_next_state_trace_from(trace, s10);
            break;
        }

        case 0x69: {
            trace->state = s15;
            add_next_state_trace_from(trace, s10);
            break;
        }

        case 0x28 ... 0x2B: {
            trace->state = s2;
            add_next_state_trace_from(trace, s11);
            break;
        }

        case 0x4A: {
            trace->state = s12;
            add_next_state_trace_from(trace, s11);
            break;
        }

        case 0x6A: {
            trace->state = s15;
            add_next_state_trace_from(trace, s11);
            break;
        }

        case 0x70: {
            trace->state = s16;
            add_next_state_trace_from(trace, s17);
            add_next_state_trace_from(trace->next, s18);
            add_next_state_trace_from(trace->next->next, s4);
            break;
        }

        case 0x71: {
            trace->state = s16;
            add_next_state_trace_from(trace, s17);
            add_next_state_trace_from(trace->next, s12);
            add_next_state_trace_from(trace->next->next, s19);
            break;
        }

        case 0x72: {  // TODO - check this. This have to be evaluated before each microinstruction
            if (flags.fz) {
                trace->state = s16;
                add_next_state_trace_from(trace, s17);
                add_next_state_trace_from(trace->next, s20);
            } else {
                trace->state = s16;
                add_next_state_trace_from(trace, s17);
            }
            break;
        }

        case 0xB0: {
            trace->state = s21;
            add_next_state_trace_from(trace, s22);
            add_next_state_trace_from(trace->next, s18);
            add_next_state_trace_from(trace->next->next, s4);
            break;
        }

        case 0x90: {
            trace->state = s21;
            add_next_state_trace_from(trace, s22);
            add_next_state_trace_from(trace->next, s12);
            add_next_state_trace_from(trace->next->next, s19);
            break;
        }

        case 0x73: {
            if (flags.fc) {
                trace->state = s16;
                add_next_state_trace_from(trace, s17);
                add_next_state_trace_from(trace->next, s20);
            } else {
                trace->state = s16;
                add_next_state_trace_from(trace, s17);
            }
            break;
        }

        case 0x74: {
            trace->state = s16;
            add_next_state_trace_from(trace, s17);
            add_next_state_trace_from(trace->next, s20);
            break;
        }

        case 0x80: {
            trace->state = s14;
            break;
        }

        case 0x81: {
            trace->state = s23;
            break;
        }

        case 0x82: {
            trace->state = s24;
            break;
        }

        case 0xC1: {
            trace->state = s26;
            add_next_state_trace_from(trace, s36);
            break;
        }

        case 0xC2: {
            trace->state = s37;
            add_next_state_trace_from(trace, s31);
            break;
        }

        case 0xC3: {
            trace->state = s16;
            add_next_state_trace_from(trace, s17);
            add_next_state_trace_from(trace->next, s38);
            break;
        }

        case 0xC4: {
            trace->state = s16;
            add_next_state_trace_from(trace, s17);
            add_next_state_trace_from(trace->next, s26);
            add_next_state_trace_from(trace->next->next, s28);
            add_next_state_trace_from(trace->next->next->next, s26);
            add_next_state_trace_from(trace->next->next->next->next, s29);
            add_next_state_trace_from(trace->next->next->next->next->next, s20);
            break;
        }

        case 0xC5: {
            trace->state = s32;
            add_next_state_trace_from(trace, s31);
            add_next_state_trace_from(trace->next, s33);
            add_next_state_trace_from(trace->next->next, s31);
            break;
        }

        case 0xC0: {
            trace->state = s32;
            add_next_state_trace_from(trace, s31);
            add_next_state_trace_from(trace->next, s33);
            add_next_state_trace_from(trace->next->next, s31);
            add_next_state_trace_from(trace->next->next->next, s34);
            add_next_state_trace_from(trace->next->next->next->next, s31);
            add_next_state_trace_from(trace->next->next->next->next->next, s35);
            break;
        }

        case 0xFF: {
            log_info("-- HALT --");
            beep();
            trace->state = s0;
            break;
        }

        default: {
            log_warn("Unhandled opcode: 0x%02X", opcode);
            trace->state = s0;  // if not found, go to s0
            break;
        }
    }
    return trace;
}

void init_cu(void) {
    register_watcher(&RI_reg_watcher);

    data_bus_topic_subscription = subscribe_to(DATA_BUS_TOPIC, &last_bus_data);
    flags_out_bus_topic_subscription = subscribe_to(FLAGS_OUTPUT_BUS_TOPIC, &last_bus_flags_out);
    state_trace = (OpStateTrace *)create_state_trace(&s0);
}

void shutdown_cu(void) {
    free(state_trace);
    unsubscribe_for(data_bus_topic_subscription);
    unsubscribe_for(flags_out_bus_topic_subscription);

    unregister_watcher(&RI_reg_watcher);
}

void run_cu(void) {  // 1 opstate per run
    log_debug("Running S%d", state_trace->state.id);
    process_state_loadbits(state_trace->state);

    switch (state_trace->state.id) {
        case S0: {
            // RI <- (PC)
            state_trace->next = create_state_trace(&s1);
        } break;
        case S1: {
            // DECODE
            state_trace->next = decode_step();
        }
        default:
            break;
    }

    run_mem();

    run_fffc();
    run_mxdir();
    run_addsub();
    // run_pc();

    run_greg();

    // run_alu(); // <<== alu is asyncroneous

    run_acumm();
    run_op2();

    run_hl();
    run_sp();

    cll_run_mxfldx();

    run_flags();

    cll_run_flagsinta();

    if (ricar_lb.value == 1) {
        // load
        if (get_bin_len(last_bus_data) > RI_reg.bit_length) {
            Error err = {.show_errno = false, .type = NOTICE_ERROR, .message = "Overflow attemping to load to RI register"};
            throw_error(err);
        }

        RI_reg.bin_value = last_bus_data;
    }

    OpStateTrace *tmp_next_state_trace = state_trace->next;
    free(state_trace);
    // if end of trace is reached, go to s0
    state_trace = tmp_next_state_trace == NULL ? create_state_trace(&s0) : tmp_next_state_trace;
}