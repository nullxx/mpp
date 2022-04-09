/*
 * File: /src/lib/components/alu.c
 * Project: mpp-cpu
 * File Created: Tuesday, 29th March 2022 10:28:57 pm
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Tuesday, 29th March 2022 10:29:23 pm
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */

#define ALU_THREAD_SEM "MPP_CPU_ALU_THREAD_SEM"
#define ALU_UPDATE_SEM "MPP_CPU_ALU_UPDATE_SEM"

#include "alu.h"

#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../constants.h"
#include "../error.h"
#include "../logger.h"
#include "../process.h"
#include "../pubsub.h"
#include "../utils.h"
#include "components.h"

static LoadBit selalu_lb = {.value = 000};

static LoadBit alubus_lb = {.value = 0};
static PubSubSubscription *acumm_output_bus_topic_subscription = NULL;
static PubSubSubscription *opt_output_bus_topic_subscription = NULL;
static ACUMMOutputBus_t last_bus_acumm_output;  // A
static OP2OutputBus_t last_bus_op2_output;      // B

static sem_t *sem1;
static sem_t *sem2;

// static void print_sem_value(const char *desc) {
//     // int s1 = 0, s2 = 0;
//     // sem_getvalue(sem1, &s1);
//     // sem_getvalue(sem2, &s2);
//     // printf("[%s] sem1: %d\n", desc, s1);
//     // printf("[%s] sem2: %d\n", desc, s2);
// }

static void update(void) {
    sem_post(sem2);  // +1
    // print_sem_value("[UPDATE] ANTES DE BLOQUEARME");
    sem_wait(sem1);  // -1
    // print_sem_value("[UPDATE] DESPUES DE BLOQUEARME");
    // sem_wait(sem2); // -1
    // print_sem_value();
}

bool set_selalu_lb(unsigned long bin) {
    const int bin_len = get_bin_len(bin);
    if (bin_len > SELALU_LOAD_BIT_SIZE_BITS) {
        return false;
    }

    selalu_lb.value = bin;
    update();
    return true;
}

void set_alubus_lb(void) {
    alubus_lb.value = 1;
    update();
}
void reset_alubus_lb(void) {
    alubus_lb.value = 0;
    update();
}

static void on_bus_acumm_output_message(PubSubMessage m) {
    last_bus_acumm_output = *(DataBus_t *)m.value;
    update();
}

static void on_bus_op2_output_message(PubSubMessage m) {
    last_bus_op2_output = *(OP2OutputBus_t *)m.value;
    update();
}

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
    publish_message_to(ALU_FZ_OUTPUT_BUS_TOPIC, &fz);

    // if result doesn't fit data bus => fc
    int fc = result_bin_len > DATA_BUS_SIZE_BITS;
    publish_message_to(ALU_FC_OUTPUT_BUS_TOPIC, &fc);

    if (fc) {
        char *result_bin_str = bin_to_str(result_bin);
        char *result_bin_str_fixed = slice_str(result_bin_str, 1, result_bin_len);  // higher bit

        result_bin = str_to_bin(result_bin_str_fixed);

        free(result_bin_str_fixed);
        free(result_bin_str);
    }

    if (alubus_lb.value == 1) {
        publish_message_to(DATA_BUS_TOPIC, &result_bin);
    }
}

static void on_exit_thread(void) { pthread_exit(NULL); }

static void *alu_thread(void) {
    signal(SIGTERM, (void *)on_exit_thread);
    while (1) {
        sem_wait(sem2); // initial 0, pause. sem_post(sem2) => +1, sem_wait(sem1) => -1 and continue executing with sem2 = 0
        // print_sem_value("ANTES DE RUN_ALU");
        run_alu();
        sem_post(sem1);
        // print_sem_value("DESPUES DE RUN_ALU");
    }

    return NULL;
}

static pthread_t thread_id;
void init_alu(void) {
    sem1 = sem_open(ALU_THREAD_SEM, O_CREAT, 0644, 0);
    sem2 = sem_open(ALU_UPDATE_SEM, O_CREAT, 0644, 0);

    if (pthread_create(&thread_id, NULL, (void *)alu_thread, NULL) == -1) {
        Error err = {.message = "Error creating alu thread", .show_errno = true, .type = FATAL_ERROR};
        return throw_error(err);
    }

    acumm_output_bus_topic_subscription = subscribe_to(ACUMM_OUTPUT_BUS_TOPIC, on_bus_acumm_output_message);
    opt_output_bus_topic_subscription = subscribe_to(OP2_OUTPUT_BUS_TOPIC, on_bus_op2_output_message);
}

void shutdown_alu(void) {
    unsubscribe_for(acumm_output_bus_topic_subscription);
    unsubscribe_for(opt_output_bus_topic_subscription);

    pthread_kill(thread_id, SIGTERM);
    pthread_join(thread_id, NULL);

    // print_sem_value("FINAL");

    sem_close(sem1);
    sem_close(sem2);

    sem_unlink(ALU_THREAD_SEM);
    sem_unlink(ALU_UPDATE_SEM);
}
