/*
 * File: /src/lib/components/alu.c
 * Project: mpp-cpu
 * File Created: Tuesday, 29th March 2022 10:28:57 pm
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Tuesday, 29th March 2022 10:29:23 pm
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */

#include "alu.h"

#include <semaphore.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../constants.h"
#include "../electronic/bus.h"
#include "../error.h"
#include "../logger.h"
#include "../pubsub.h"
#include "../thread.h"
#include "../utils.h"
#include "components.h"

static LoadBit selalu_lb = {.value = 000};

static LoadBit alubus_lb = {.value = 0};
static PubSubSubscription *acumm_output_bus_topic_subscription = NULL;
static PubSubSubscription *opt_output_bus_topic_subscription = NULL;
static Bus_t *last_bus_acumm_output = NULL;  // A
static Bus_t *last_bus_op2_output = NULL;    // B

bool set_selalu_lb(unsigned long bin) {
    const int bin_len = get_used_bits(bin);
    if (bin_len > SELALU_LOAD_BIT_SIZE_BITS) {
        return false;
    }

    selalu_lb.value = bin;
    return true;
}

void set_alubus_lb(void) { alubus_lb.value = 1; }
void reset_alubus_lb(void) { alubus_lb.value = 0; }

void run_alu(void) {
    update_bus_data(last_bus_acumm_output);
    update_bus_data(last_bus_op2_output);

    int sel_alu = selalu_lb.value;
    int bus_acumm_output = word_to_int(last_bus_acumm_output->next_value);
    int bus_op2_output = word_to_int(last_bus_op2_output->next_value);
    int result = 0;

    switch (sel_alu) {
        case SUM: {
            // A+B
            result = bus_acumm_output + bus_op2_output;
            break;
        }

        case SUB: {
            // A-B
            result = bus_acumm_output - bus_op2_output;
            break;
        }

        case AND: {
            // A AND B
            result = bus_acumm_output & bus_op2_output;
            break;
        }

        case OR: {
            // A OR B
            result = bus_acumm_output | bus_op2_output;
            break;
        }

        case XOR: {
            // A XOR B
            result = bus_acumm_output ^ bus_op2_output;
            break;
        }

        case NOT: {
            // NOT A
            result = ~bus_acumm_output;
            break;
        }

        case TRANSPARENT: {
            // transparent step
            result = bus_op2_output;
            break;
        }

        case INCREMENT: {
            // B+1
            result = bus_op2_output + 1;
            break;
        }

        default:
            break;
    }

    const int result_bits = get_used_bits(result);

    // if result == 0 => fz
    int fz = result == 0;
    publish_message_to(ALU_FZ_OUTPUT_BUS_TOPIC, int_to_word(fz));

    // if result doesn't fit data bus => fc
    int fc = result_bits > DATA_BUS_SIZE_BITS;
    publish_message_to(ALU_FC_OUTPUT_BUS_TOPIC, int_to_word(fc));

    if (fc) {
        // get rid of the extra higher bits
        result = result << (result_bits - DATA_BUS_SIZE_BITS);
    }

    if (alubus_lb.value == 1) {
        publish_message_to(DATA_BUS_TOPIC, int_to_word(result));
    }
}

void init_alu(void) {
    last_bus_acumm_output = create_bus_data();
    last_bus_op2_output = create_bus_data();
    acumm_output_bus_topic_subscription = subscribe_to(ACUMM_OUTPUT_BUS_TOPIC, last_bus_acumm_output);
    opt_output_bus_topic_subscription = subscribe_to(OP2_OUTPUT_BUS_TOPIC, last_bus_op2_output);
}

void shutdown_alu(void) {
    unsubscribe_for(acumm_output_bus_topic_subscription);
    unsubscribe_for(opt_output_bus_topic_subscription);
    destroy_bus_data(last_bus_acumm_output);
    destroy_bus_data(last_bus_op2_output);
}
