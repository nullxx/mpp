/*
 * File: /src/linker.c
 * Project: mpp-cpu
 * File Created: Saturday, 16th April 2022 8:43:06 pm
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Saturday, 16th April 2022 8:43:10 pm
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */

#define CLOCK_TICK_SLEEP_US 100000
#include "linker.h"

#include <math.h>
#include <stdlib.h>
#include <unistd.h>

#include "lib/components/cu/cu.h"
#include "lib/components/mem.h"
#include "lib/constants.h"
#include "lib/electronic/bus.h"
#include "lib/logger.h"
#include "lib/pubsub.h"
#include "lib/utils.h"

static Bus_t *actual_status_Q4_bus = NULL;
static Bus_t *actual_status_Q3_bus = NULL;
static Bus_t *actual_status_Q2_bus = NULL;
static Bus_t *actual_status_Q1_bus = NULL;
static Bus_t *actual_status_Q0_bus = NULL;
static Bus_t *data_bus = NULL;
static Bus_t *dir_bus = NULL;
static Bus_t *control_bus = NULL;
static Bus_t *d4flipflop_bus;
static Bus_t *d3flipflop_bus;
static Bus_t *d2flipflop_bus;
static Bus_t *d1flipflop_bus;
static Bus_t *d0flipflop_bus;
static PubSubSubscription *actual_status_Q4_subscription = NULL;
static PubSubSubscription *actual_status_Q3_subscription = NULL;
static PubSubSubscription *actual_status_Q2_subscription = NULL;
static PubSubSubscription *actual_status_Q1_subscription = NULL;
static PubSubSubscription *actual_status_Q0_subscription = NULL;
static PubSubSubscription *data_bus_subscription = NULL;
static PubSubSubscription *dir_bus_subscription = NULL;
static PubSubSubscription *control_bus_subscription = NULL;
static PubSubSubscription *d4_bus_topic_subscription = NULL;
static PubSubSubscription *d3_bus_topic_subscription = NULL;
static PubSubSubscription *d2_bus_topic_subscription = NULL;
static PubSubSubscription *d1_bus_topic_subscription = NULL;
static PubSubSubscription *d0_bus_topic_subscription = NULL;

void (*update_ui)(void) = NULL;
void (*run_end)(void) = NULL;
RunConfig run_conf;

char *last_current_state = NULL;
char *last_bus_data = NULL;
char *last_bus_dir = NULL;
char *last_bus_control = NULL;
MemUpdated last_mem_updated = {.values_count = 0, .values = NULL};

int running = 0;

void set_update_ui(void (*update_ui_func)(void)) { update_ui = update_ui_func; }
void set_run_end(void (*run_end_func)(void)) { run_end = run_end_func; }

void linker_init(void) {
    actual_status_Q4_bus = create_bus_data();
    actual_status_Q3_bus = create_bus_data();
    actual_status_Q2_bus = create_bus_data();
    actual_status_Q1_bus = create_bus_data();
    actual_status_Q0_bus = create_bus_data();
    data_bus = create_bus_data();
    dir_bus = create_bus_data();
    control_bus = create_bus_data();
    d4flipflop_bus = create_bus_data();
    d3flipflop_bus = create_bus_data();
    d2flipflop_bus = create_bus_data();
    d1flipflop_bus = create_bus_data();
    d0flipflop_bus = create_bus_data();

    actual_status_Q4_subscription = subscribe_to(CU_SEQ_ACTUAL_STATUS_Q4_BUS_TOPIC, actual_status_Q4_bus);
    actual_status_Q3_subscription = subscribe_to(CU_SEQ_ACTUAL_STATUS_Q3_BUS_TOPIC, actual_status_Q3_bus);
    actual_status_Q2_subscription = subscribe_to(CU_SEQ_ACTUAL_STATUS_Q2_BUS_TOPIC, actual_status_Q2_bus);
    actual_status_Q1_subscription = subscribe_to(CU_SEQ_ACTUAL_STATUS_Q1_BUS_TOPIC, actual_status_Q1_bus);
    actual_status_Q0_subscription = subscribe_to(CU_SEQ_ACTUAL_STATUS_Q0_BUS_TOPIC, actual_status_Q0_bus);
    data_bus_subscription = subscribe_to(DATA_BUS_TOPIC, data_bus);
    dir_bus_subscription = subscribe_to(DIR_BUS_TOPIC_1, dir_bus);
    control_bus_subscription = subscribe_to(CONTROL_BUS_TOPIC, control_bus);
    d4_bus_topic_subscription = subscribe_to(CU_SEQ_OUTPUT_D4_BUS_TOPIC, d4flipflop_bus);
    d3_bus_topic_subscription = subscribe_to(CU_SEQ_OUTPUT_D3_BUS_TOPIC, d3flipflop_bus);
    d2_bus_topic_subscription = subscribe_to(CU_SEQ_OUTPUT_D2_BUS_TOPIC, d2flipflop_bus);
    d1_bus_topic_subscription = subscribe_to(CU_SEQ_OUTPUT_D1_BUS_TOPIC, d1flipflop_bus);
    d0_bus_topic_subscription = subscribe_to(CU_SEQ_OUTPUT_D0_BUS_TOPIC, d0flipflop_bus);
}

void linker_shutdown(void) {
    running = 0;
    if (last_current_state != NULL) {
        free(last_current_state);
        last_current_state = NULL;
    }
    if (last_bus_data != NULL) {
        free(last_bus_data);
        last_bus_data = NULL;
    }
    if (last_bus_dir != NULL) {
        free(last_bus_dir);
        last_bus_dir = NULL;
    }
    if (last_bus_control != NULL) {
        free(last_bus_control);
        last_bus_control = NULL;
    }
    if (last_mem_updated.values != NULL) {
        free(last_mem_updated.values);
        last_mem_updated.values = NULL;
    }

    unsubscribe_for(actual_status_Q4_subscription);
    unsubscribe_for(actual_status_Q3_subscription);
    unsubscribe_for(actual_status_Q2_subscription);
    unsubscribe_for(actual_status_Q1_subscription);
    unsubscribe_for(actual_status_Q0_subscription);
    unsubscribe_for(data_bus_subscription);
    unsubscribe_for(dir_bus_subscription);
    unsubscribe_for(control_bus_subscription);
    unsubscribe_for(d4_bus_topic_subscription);
    unsubscribe_for(d3_bus_topic_subscription);
    unsubscribe_for(d2_bus_topic_subscription);
    unsubscribe_for(d1_bus_topic_subscription);
    unsubscribe_for(d0_bus_topic_subscription);

    destroy_bus_data(actual_status_Q4_bus);
    destroy_bus_data(actual_status_Q3_bus);
    destroy_bus_data(actual_status_Q2_bus);
    destroy_bus_data(actual_status_Q1_bus);
    destroy_bus_data(actual_status_Q0_bus);
    destroy_bus_data(data_bus);
    destroy_bus_data(dir_bus);
    destroy_bus_data(control_bus);
    destroy_bus_data(d4flipflop_bus);
    destroy_bus_data(d3flipflop_bus);
    destroy_bus_data(d2flipflop_bus);
    destroy_bus_data(d1flipflop_bus);
    destroy_bus_data(d0flipflop_bus);
}

static int linker_get_rom_pos(void) {
    Word w;
    initialize_word(&w, 0);
    w.bits[4] = actual_status_Q4_bus->next_value.bits[0];
    w.bits[3] = actual_status_Q3_bus->next_value.bits[0];
    w.bits[2] = actual_status_Q2_bus->next_value.bits[0];
    w.bits[1] = actual_status_Q1_bus->next_value.bits[0];
    w.bits[0] = actual_status_Q0_bus->next_value.bits[0];

    int rom_pos = word_to_int(w);
    return rom_pos;
}

static int linker_get_next_state(void) {
    Word w;
    initialize_word(&w, 0);
    w.bits[4] = d4flipflop_bus->next_value.bits[0];
    w.bits[3] = d3flipflop_bus->next_value.bits[0];
    w.bits[2] = d2flipflop_bus->next_value.bits[0];
    w.bits[1] = d1flipflop_bus->next_value.bits[0];
    w.bits[0] = d0flipflop_bus->next_value.bits[0];

    int next_state = word_to_int(w);
    return next_state;
}

void linker_cancel_run(void) { running = 0; }

int linker_is_running(void) { return running; }

void linker_run(RunConfig conf) {
    if (running) return;
    running = 1;
    run_conf = conf;
    log_debug("run_conf.type: %d", run_conf.type);

    double cycle_time = 0;
    switch (run_conf.type) {
        case STATE:
            // one cycle => one state
            cycle_time = run_clock_cycle();
            update_ui();
            break;
        case MICRO_INSTRUCTION:
            // runs until the next state is next
            while (1) {
                if (running == 0) break;
                usleep(CLOCK_TICK_SLEEP_US);
                cycle_time += run_clock_cycle();
                update_ui();
                int next_state = linker_get_next_state();
                if (next_state == 0) break;
            }
            break;
        case PROGRAM:
            // runs until RI = 0xFF
            while (1) {
                if (running == 0) break;
                usleep(CLOCK_TICK_SLEEP_US);
                cycle_time += run_clock_cycle();
                update_ui();
                int ri = word_to_int(get_watchers(1).RI->reg->value);
                if (ri == pow(2, MEM_VALUE_SIZE_BITS) - 1) break;
            }
            break;
        default:
            break;
    }

    log_debug("cycle_time: %f", cycle_time);
    running = 0;
    run_end();
}

Watchers linker_get_watchers(void) { return get_watchers(0); }

char *linker_get_current_state_updated(void) {
    // rom_pos == state
    int rom_pos = linker_get_rom_pos();
    char *rom_pos_str = num_to_str(rom_pos);
    char *current_state = str_concat("S", rom_pos_str);

    if (last_current_state != NULL) {
        free(last_current_state);
    }
    last_current_state = current_state;

    return current_state;
}

char *linker_get_data_bus_updated(void) {
    char *bus_data_hex = int_to_hex("0x", word_to_int(data_bus->next_value));

    if (last_bus_data != NULL) {
        free(last_bus_data);
    }

    last_bus_data = bus_data_hex;
    return bus_data_hex;
}

char *linker_get_dir_bus_updated(void) {
    char *bus_dir_hex = int_to_hex("0x", word_to_int(dir_bus->next_value));

    if (last_bus_dir != NULL) {
        free(last_bus_dir);
    }

    last_bus_dir = bus_dir_hex;
    return bus_dir_hex;
}

char *linker_get_control_bus_updated(void) {
    char *bus_control_hex = int_to_hex("0x", word_to_int(control_bus->next_value));

    if (last_bus_control != NULL) {
        free(last_bus_control);
    }

    last_bus_control = bus_control_hex;
    return bus_control_hex;
}

MemUpdated linker_get_mem_updated(void) {
    Mem mem = get_mem();
    MemUpdated mem_updated = {.values_count = mem.values_count, .values = malloc(sizeof(MemValueUpdated) * mem.values_count)};

    for (int i = 0; i < mem.values_count; i++) {
        MemValueUpdated mem_value_updated;
        mem_value_updated.offset = mem.values[i].offset;
        mem_value_updated.value = mem.values[i].value;
        mem_value_updated.offset_hex = int_to_hex("", mem.values[i].offset);
        mem_value_updated.value_hex = int_to_hex("", mem.values[i].value);
        mem_updated.values[i] = mem_value_updated;
    }

    for (int i = 0; i < last_mem_updated.values_count; i++) {
        free(last_mem_updated.values[i].offset_hex);
        free(last_mem_updated.values[i].value_hex);
    }
    free(last_mem_updated.values);

    last_mem_updated = mem_updated;
    return mem_updated;
}