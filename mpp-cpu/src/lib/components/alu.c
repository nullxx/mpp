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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../constants.h"
#include "../electronic/bus.h"
#include "../error.h"
#include "../logger.h"
#include "../pubsub.h"
#include "../thread.h"
#include "../utils.h"
#include "components.h"

static Bus_t *last_bus_acumm_output = NULL;  // A
static Bus_t *last_bus_op2_output = NULL;    // B
static Bus_t *control_bus = NULL;

static PubSubSubscription *acumm_output_bus_topic_subscription = NULL;
static PubSubSubscription *opt_output_bus_topic_subscription = NULL;
static PubSubSubscription *control_bus_topic_subscription = NULL;

void run_alu(void) {
    update_bus_data(last_bus_acumm_output);
    update_bus_data(last_bus_op2_output);
    update_bus_data(control_bus);

    Word selalu_lb;
    Word alubus_lb;
    initialize_word(&selalu_lb, 0);
    initialize_word(&alubus_lb, 0);

    selalu_lb.bits[0] = control_bus->current_value.bits[CONTROL_BUS_SELALU_0_BIT_POSITION];
    selalu_lb.bits[1] = control_bus->current_value.bits[CONTROL_BUS_SELALU_1_BIT_POSITION];
    selalu_lb.bits[2] = control_bus->current_value.bits[CONTROL_BUS_SELALU_2_BIT_POSITION];

    alubus_lb.bits[0] = control_bus->current_value.bits[CONTROL_BUS_ALUBUS_BIT_POSITION];

    int sel_alu = word_to_int(selalu_lb);
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
        Word word_result = int_to_word(result);
        for (int i = WORD_SIZE_BIT - 1; i <= 0; i++) {
            if (word_result.bits[i] == 1) {
                word_result.bits[i] = 0;
                break;
            }
        }

        result = word_to_int(word_result);
    }

    if (word_to_int(alubus_lb) == 1) {
        publish_message_to(DATA_BUS_TOPIC, int_to_word(result));
    }
}

void init_alu(void) {
    last_bus_acumm_output = create_bus_data();
    last_bus_op2_output = create_bus_data();
    control_bus = create_bus_data();
    acumm_output_bus_topic_subscription = subscribe_to(ACUMM_OUTPUT_BUS_TOPIC, last_bus_acumm_output);
    opt_output_bus_topic_subscription = subscribe_to(OP2_OUTPUT_BUS_TOPIC, last_bus_op2_output);
    control_bus_topic_subscription = subscribe_to(CONTROL_BUS_TOPIC, control_bus);
}

void shutdown_alu(void) {
    unsubscribe_for(acumm_output_bus_topic_subscription);
    unsubscribe_for(opt_output_bus_topic_subscription);
    unsubscribe_for(control_bus_topic_subscription);
    destroy_bus_data(last_bus_acumm_output);
    destroy_bus_data(last_bus_op2_output);
    destroy_bus_data(control_bus);
}
