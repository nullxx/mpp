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

#include "../thread.h"
#include <semaphore.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../constants.h"
#include "../error.h"
#include "../logger.h"

#include "../pubsub.h"
#include "../utils.h"
#include "components.h"

static LoadBit selalu_lb = {.value = 000};

static LoadBit alubus_lb = {.value = 0};
static PubSubSubscription *acumm_output_bus_topic_subscription = NULL;
static PubSubSubscription *opt_output_bus_topic_subscription = NULL;
static Bus_t last_bus_acumm_output;  // A
static Bus_t last_bus_op2_output;      // B

bool set_selalu_lb(unsigned long bin) {
    const int bin_len = get_bin_len(bin);
    if (bin_len > SELALU_LOAD_BIT_SIZE_BITS) {
        return false;
    }

    selalu_lb.value = bin;
    return true;
}

void set_alubus_lb(void) { alubus_lb.value = 1; }
void reset_alubus_lb(void) { alubus_lb.value = 0; }

void run_alu(void) {
    int sel_alu_int = bin_to_int(selalu_lb.value);
    unsigned long long result_bin = 0;

    switch (sel_alu_int) {
        case SUM: {
            // A+B
            const int sum_result = bin_to_int(last_bus_acumm_output) + bin_to_int(last_bus_op2_output);
            result_bin = int_to_bin(sum_result, MAX_CALC_BIN_SIZE_BITS);
            break;
        }

        case SUB: {
            // A-B
            const int sub_result = bin_to_int(last_bus_acumm_output) - bin_to_int(last_bus_op2_output);
            result_bin = int_to_bin(sub_result, MAX_CALC_BIN_SIZE_BITS);
            break;
        }

        case AND: {
            // A AND B
            const int bwe_and_result = last_bus_acumm_output & last_bus_op2_output;
            result_bin = bwe_and_result;
            break;
        }

        case OR: {
            // A OR B
            const int bwe_or_result = last_bus_acumm_output | last_bus_op2_output;
            result_bin = bwe_or_result;
            break;
        }

        case XOR: {
            // A XOR B
            const int bwe_xor_result = last_bus_acumm_output ^ last_bus_op2_output;
            result_bin = bwe_xor_result;
            break;
        }

        case NOT: {
            // NOT A
            const int bwe_not_result = ~last_bus_acumm_output;
            result_bin = bwe_not_result;
            break;
        }

        case TRANSPARENT: {
            // transparent step
            result_bin = last_bus_op2_output;
            break;
        }

        case INCREMENT: {
            // B+1
            const int sum_result = bin_to_int(last_bus_op2_output) + 1;
            result_bin = int_to_bin(sum_result, MAX_CALC_BIN_SIZE_BITS);
            break;
        }

        default:
            break;
    }

    const int result_bin_len = get_bin_len(result_bin);

    // if result == 0 => fz
    int fz = result_bin == 0;
    publish_message_to(ALU_FZ_OUTPUT_BUS_TOPIC, fz);

    // if result doesn't fit data bus => fc
    int fc = result_bin_len > DATA_BUS_SIZE_BITS;
    publish_message_to(ALU_FC_OUTPUT_BUS_TOPIC, fc);

    if (fc) {
        char *result_bin_str = bin_to_str(result_bin);
        char *result_bin_str_fixed = slice_str(result_bin_str, 1, result_bin_len);  // higher bit

        result_bin = str_to_bin(result_bin_str_fixed);

        free(result_bin_str_fixed);
        free(result_bin_str);
    }

    if (alubus_lb.value == 1) {
        publish_message_to(DATA_BUS_TOPIC, result_bin);
    }
}

int alu_thread_runnable = 1;
static void *alu_thread(void) {
    while (alu_thread_runnable) {
        run_alu();
    }

    return NULL;
}

static pthread_t *thread_id = NULL;
void init_alu(void) {
    acumm_output_bus_topic_subscription = subscribe_to(ACUMM_OUTPUT_BUS_TOPIC, &last_bus_acumm_output);
    opt_output_bus_topic_subscription = subscribe_to(OP2_OUTPUT_BUS_TOPIC, &last_bus_op2_output);

    if ((thread_id = create_pthread(NULL, (void *)alu_thread, NULL)) == NULL) {
        Error err = {.message = "Error creating alu thread", .show_errno = true, .type = FATAL_ERROR};
        return throw_error(err);
    }
}

void shutdown_alu(void) {
    alu_thread_runnable = 0;
    pthread_join(*thread_id, NULL);

    unsubscribe_for(acumm_output_bus_topic_subscription);
    unsubscribe_for(opt_output_bus_topic_subscription);
}
