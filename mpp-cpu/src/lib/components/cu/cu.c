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

#include "../../clock.h"
#include "../../constants.h"
#include "../../electronic/bus.h"
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
#include "dxflipflop.h"
#include "mxreg_jnr.h"
#include "rom.h"
#include "seq.h"
#include "seq_fgs_jnr.h"
#include "states.h"

Register RI_reg = {.bit_length = RI_REG_SIZE_BIT};

RegisterWatcher RI_reg_watcher = {.name = "RI", .reg = &RI_reg};

static Bus_t *last_bus_data = NULL;
static Bus_t *last_bus_flags_out = NULL;
static Bus_t *control_bus = NULL;
static PubSubSubscription *data_bus_topic_subscription = NULL;
static PubSubSubscription *flags_out_bus_topic_subscription = NULL;
static PubSubSubscription *control_bus_topic_subscription = NULL;

// clock tick generator must be inside cu, but as this is a simulator, the clock will be managed from the window to decrease CPU usage
double run_clock_cycle(void) {
    while (1) {
        clock_t start = clock();
        int clock_tick = get_clock_tick();
        run_cu(clock_tick);
        clock_t end = clock();

        if (clock_tick == 1) {
            double seconds_spent = (double)(end - start) / CLOCKS_PER_SEC;
            // log_info("Cycle time: %fs => %f KHz", seconds_spent, (1 / seconds_spent) / 1000);

            // log_watchers();
            return seconds_spent;
        }
    }
}

void init_cu(void) {
    initialize_word(&RI_reg.value, 0);

    last_bus_data = create_bus_data();
    last_bus_flags_out = create_bus_data();
    control_bus = create_bus_data();
    data_bus_topic_subscription = subscribe_to(DATA_BUS_TOPIC, last_bus_data);
    flags_out_bus_topic_subscription = subscribe_to(FLAGS_OUTPUT_BUS_TOPIC, last_bus_flags_out);
    control_bus_topic_subscription = subscribe_to(CONTROL_BUS_TOPIC, control_bus);

    register_watcher(&RI_reg_watcher);

    init_cu_seq_b_jnr();
    init_cu_mxreg_jnr();
    init_cu_seq();
    init_cu_dxflipflop();
    init_cu_rom();
}

void shutdown_cu(void) {
    unregister_watcher(&RI_reg_watcher);

    shutdown_cu_seq_b_jnr();
    shutdown_cu_mxreg_jnr();
    shutdown_cu_seq();
    shutdown_cu_dxflipflop();
    shutdown_cu_rom();

    unsubscribe_for(data_bus_topic_subscription);
    unsubscribe_for(flags_out_bus_topic_subscription);
    unsubscribe_for(control_bus_topic_subscription);
    destroy_bus_data(last_bus_data);
    destroy_bus_data(last_bus_flags_out);
    destroy_bus_data(control_bus);
}

void run_asyncronus_components(void) {
    run_addsub();
    run_mxdir();
    run_mxreg();

    for (size_t i = 0; i < ALU_MEM_RELATION; i++) {
        run_mem();
        run_alu();
    }
}

void run_sync_comp(void (*run_comp_fn)(void)) {
    run_comp_fn();
    run_asyncronus_components();
}

void run_cu(int clk) {  // 1 opstate per run
    log_debug("Clock: %d", clk);
    if (clk == 1) run_cu_seq_b_jnr();
    if (clk == 1) run_cu_mxreg_jnr();
    if (clk == 1) run_cu_seq();
    if (clk == 0) run_cu_dxflipflop();
    if (clk == 1) run_cu_rom();

    if (clk == 1) run_asyncronus_components();

    if (clk == 1) {
        update_bus_data(last_bus_data);
        update_bus_data(last_bus_flags_out);
        update_bus_data(control_bus);

        if (control_bus->current_value.bits[CONTROL_BUS_RICAR_BIT_POSITION] == 1) {
            // load
            int b = get_used_bits(word_to_int(last_bus_data->current_value));
            if (b > RI_reg.bit_length) {
                Error err = {.show_errno = false, .type = NOTICE_ERROR, .message = "Overflow attemping to load to RI register"};
                throw_error(err);
            }

            RI_reg.value = last_bus_data->current_value;
        }

        publish_message_to(CU_RI_OUTPUT_BUS_TOPIC, RI_reg.value);
    }

    // run_mem(); // <<==  is asyncroneous

    // run_fffc();
    if (clk == 1) run_sync_comp(run_fffc);

    // run_mxdir(); // <<==  is asyncroneous
    // run_addsub(); // <<== is asyncroneous

    // run_acumm();
    if (clk == 1) run_sync_comp(run_acumm);

    // run_op2();
    if (clk == 1) run_sync_comp(run_op2);

    // run_greg();
    if (clk == 1) run_sync_comp(run_greg);

    // run_alu(); // <<== is asyncroneous

    // run_hl();
    if (clk == 1) run_sync_comp(run_hl);

    // run_sp();
    if (clk == 1) run_sync_comp(run_sp);

    // cll_run_mxfldx();
    if (clk == 1) run_sync_comp(cll_run_mxfldx);

    // run_flags();
    if (clk == 1) run_sync_comp(run_flags);

    // cll_run_flagsinta();
    if (clk == 1) run_sync_comp(cll_run_flagsinta);

    // run_pc();
    if (clk == 1) run_sync_comp(run_pc);
}