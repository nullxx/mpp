/*
 * File: /src/lib/components/cu/rom.c
 * Project: mpp-cpu
 * File Created: Thursday, 14th April 2022 6:30:44 pm
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Thursday, 14th April 2022 6:31:09 pm
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */

#include "rom.h"
#include "../../pubsub.h"
#include "../../electronic/bus.h"
#include "../../utils.h"

#define X -1
#define XX -2
#define XXX -3

#define CU_SIGNAL_ROWS_COUNT 39
#define CU_SIGNAL_COLS_COUNT 24
#define SIGNAL_SIZE_BITS 31

Bus_t actual_status_bus;
PubSubSubscription *actual_status_bus_subscription;

const int ROM[CU_SIGNAL_ROWS_COUNT][CU_SIGNAL_COLS_COUNT] = { // this are the signals 
    {XXX, XX, 0b00, XX, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, X, 0, 0, 0, 0, 0, 0},
    {XXX, XX, XX, XX, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, X, 0, 0, 0, 0, 0, 0},
    {XXX, 0b00, XX, XX, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, X, 0, 0, 0, 0, 0, 0},
    {0b110, 0b01, XX, XX, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, X, 0, 0, 0, 0, 0, 0},
    {0b110, XX, XX, XX, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, X, 0, 0, 0, 0, 0, 0},
    {0b000, XX, XX, XX, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0},
    {0b001, XX, XX, XX, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0},
    {0b001, XX, XX, XX, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
    {0b111, 0b00, XX, XX, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0},
    {0b010, XX, XX, XX, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0},
    {0b011, XX, XX, XX, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0},
    {0b100, XX, XX, XX, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0},
    {XXX, XX, XX, XX, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, X, 0, 0, 0, 0, 0, 0},
    {0b111, XX, XX, XX, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0},
    {0b101, XX, XX, XX, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0},
    {XXX, XX, 0b00, 0b01, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, X, 0, 0, 0, 0, 0, 0},
    {XXX, XX, 0b00, 0b01, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, X, 0, 1, 0, 0, 0, 0},
    {XXX, XX, 0b00, 0b01, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, X, 0, 0, 1, 0, 0, 0},
    {XXX, XX, 0b10, XX, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, X, 0, 0, 0, 0, 0, 0},
    {0b110, XX, 0b10, XX, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, X, 0, 1, 0, 0, 0, 0},
    {XXX, XX, 0b10, 0b00, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, X, 0, 0, 0, 0, 0, 0},
    {XXX, 0b10, XX, XX, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, X, 0, 1, 0, 0, 0, 0},
    {XXX, 0b11, XX, XX, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, X, 0, 0, 1, 0, 0, 0},
    {XXX, XX, X, XX, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, X, 0, 0, 0, 0, 0, 0},
    {XXX, XX, XX, XX, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
    {XXX, XX, XX, XX, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, X, 0, 0, 0, 0, 0, 0},
    {XXX, XX, 0b01, 0b11, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, X, 0, 0, 0, 0, 0, 1},
    {XXX, XX, 0b01, XX, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, X, 0, 0, 0, 0, 0, 0},
    {XXX, XX, 0b01, XX, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, X, 0, 0, 0, 0, 0, 0},
    {XXX, XX, 0b01, XX, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, X, 0, 0, 0, 0, 0, 0},
    {XXX, XX, 0b11, 0b00, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, X, 0, 0, 0, 0, 0, 0},
    {XXX, XX, 0b01, 0b01, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, X, 0, 0, 0, 0, 0, 1},
    {XXX, XX, 0b01, XX, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, X, 0, 0, 0, 0, 1, 0},
    {XXX, XX, 0b01, XX, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, X, 0, 0, 0, 1, 0, 0},
    {XXX, XX, 0b01, XX, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
    {XXX, XX, XX, XX, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, X, 0, 0, 0, 0, 0, 0},
    {XXX, XX, 0b01, XX, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, X, 0, 0, 0, 0, 0, 0},
    {XXX, XX, 0b01, XX, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, X, 0, 0, 0, 0, 0, 0},
    {XXX, XX, 0b10, 0b00, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, X, 0, 0, 0, 0, 0, 1}
};

void init_cu_rom(void) {
    actual_status_bus_subscription = subscribe_to(CU_SEQ_ACTUAL_STATUS_BUS_TOPIC, &actual_status_bus);
}

void run_cu_rom(void) {
    int rom_pos_dec = bin_to_dec(actual_status_bus.next_value);
    
    if (rom_pos_dec < 0 || rom_pos_dec > CU_SIGNAL_ROWS_COUNT) return;

    Bin bin;
    for (int i = 0; i < CU_SIGNAL_COLS_COUNT; i++) {
        if (i == 0) {
            bin = ROM[rom_pos_dec][i];
             continue;
        }

        bin = concatenate(bin, ROM[rom_pos_dec][i]);
    }

    // FIXME this is a fucking shit

    publish_message_to(CONTROL_BUS_TOPIC, bin);
}

void shutdown_cu_rom(void) {
    unsubscribe_for(actual_status_bus_subscription);
}