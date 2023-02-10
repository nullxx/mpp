#define MUNIT_ENABLE_ASSERT_ALIASES
#include "../../../../src/lib/components/alu.h"

#include <stdio.h>

#include "../../../../src/lib/constants.h"
#include "../../../../src/lib/logger.h"
#include "../../../../src/lib/pubsub.h"
#include "../../../../src/lib/watcher.h"
#include "../../../_munit/munit.h"

static PubSubSubscription *acumm_output_bus_topic_subscription = NULL;
static PubSubSubscription *opt_output_bus_topic_subscription = NULL;
static PubSubSubscription *control_bus_topic_subscription = NULL;
static PubSubSubscription *data_bus_topic_subscription = NULL;
static PubSubSubscription *alu_fz_output_bus_subscription = NULL;
static PubSubSubscription *alu_fc_output_bus_subscription = NULL;

static Bus_t *last_bus_acumm_output = NULL;  // A
static Bus_t *last_bus_op2_output = NULL;    // B
static Bus_t *control_bus = NULL;
static Bus_t *data_bus = NULL;
static Bus_t *alu_fz_output_bus = NULL;
static Bus_t *alu_fc_output_bus = NULL;

static void setup(void) {
    last_bus_acumm_output = create_bus_data();
    last_bus_op2_output = create_bus_data();
    control_bus = create_bus_data();
    data_bus = create_bus_data();
    alu_fz_output_bus = create_bus_data();
    alu_fc_output_bus = create_bus_data();

    acumm_output_bus_topic_subscription = subscribe_to(ACUMM_OUTPUT_BUS_TOPIC, last_bus_acumm_output);
    opt_output_bus_topic_subscription = subscribe_to(OP2_OUTPUT_BUS_TOPIC, last_bus_op2_output);
    control_bus_topic_subscription = subscribe_to(CONTROL_BUS_TOPIC, control_bus);
    data_bus_topic_subscription = subscribe_to(DATA_BUS_TOPIC, data_bus);
    alu_fz_output_bus_subscription = subscribe_to(ALU_FZ_OUTPUT_BUS_TOPIC, alu_fz_output_bus);
    alu_fc_output_bus_subscription = subscribe_to(ALU_FC_OUTPUT_BUS_TOPIC, alu_fc_output_bus);

    assert_ptr_not_null(last_bus_acumm_output);
    assert_ptr_not_null(last_bus_op2_output);
    assert_ptr_not_null(control_bus);
    assert_ptr_not_null(data_bus);
    assert_ptr_not_null(alu_fz_output_bus);
    assert_ptr_not_null(alu_fc_output_bus);

    assert_ptr_not_null(acumm_output_bus_topic_subscription);
    assert_ptr_not_null(opt_output_bus_topic_subscription);
    assert_ptr_not_null(control_bus_topic_subscription);
    assert_ptr_not_null(data_bus_topic_subscription);
    assert_ptr_not_null(alu_fz_output_bus_subscription);
    assert_ptr_not_null(alu_fc_output_bus_subscription);

    init_alu();
}

static void test_run_alu_sum(void) {
    Word wc = int_to_word(0);

    wc.bits[CONTROL_BUS_SELALU_0_BIT_POSITION] = 0;
    wc.bits[CONTROL_BUS_SELALU_1_BIT_POSITION] = 0;
    wc.bits[CONTROL_BUS_SELALU_2_BIT_POSITION] = 0;

    wc.bits[CONTROL_BUS_ALUBUS_BIT_POSITION] = 1;  // release to databus

    publish_message_to(CONTROL_BUS_TOPIC, wc);

    Word A = int_to_word(1);
    Word B = int_to_word(1);
    publish_message_to(ACUMM_OUTPUT_BUS_TOPIC, A);
    publish_message_to(OP2_OUTPUT_BUS_TOPIC, B);

    run_alu();

    assert_int(word_to_int(data_bus->next_value), ==, word_to_int(A) + word_to_int(B));
    assert_int(word_to_int(alu_fc_output_bus->next_value), ==, 0);
    assert_int(word_to_int(alu_fz_output_bus->next_value), ==, 0);

    // ---- check overflow

    A = int_to_word(0xFF);
    B = int_to_word(2);
    publish_message_to(ACUMM_OUTPUT_BUS_TOPIC, A);
    publish_message_to(OP2_OUTPUT_BUS_TOPIC, B);

    run_alu();

    assert_int(word_to_int(data_bus->next_value), ==, 0x01);
    assert_int(word_to_int(alu_fc_output_bus->next_value), ==, 1);
    assert_int(word_to_int(alu_fz_output_bus->next_value), ==, 0);
}

static void test_run_alu_sub(void) {
    Word wc = int_to_word(0);

    wc.bits[CONTROL_BUS_SELALU_0_BIT_POSITION] = 1;
    wc.bits[CONTROL_BUS_SELALU_1_BIT_POSITION] = 0;
    wc.bits[CONTROL_BUS_SELALU_2_BIT_POSITION] = 0;

    wc.bits[CONTROL_BUS_ALUBUS_BIT_POSITION] = 1;  // release to databus

    publish_message_to(CONTROL_BUS_TOPIC, wc);

    Word A = int_to_word(1);
    Word B = int_to_word(1);
    publish_message_to(ACUMM_OUTPUT_BUS_TOPIC, A);
    publish_message_to(OP2_OUTPUT_BUS_TOPIC, B);

    run_alu();

    assert_int(word_to_int(data_bus->next_value), ==, word_to_int(A) - word_to_int(B));
    assert_int(word_to_int(alu_fc_output_bus->next_value), ==, 0);
    assert_int(word_to_int(alu_fz_output_bus->next_value), ==, 1);

    // ---- check overflow

    A = int_to_word(0);
    B = int_to_word(2);
    publish_message_to(ACUMM_OUTPUT_BUS_TOPIC, A);
    publish_message_to(OP2_OUTPUT_BUS_TOPIC, B);

    run_alu();

    assert_int(word_to_int(data_bus->next_value), ==, 0xFE);
    assert_int(word_to_int(alu_fc_output_bus->next_value), ==, 1);
    assert_int(word_to_int(alu_fz_output_bus->next_value), ==, 0);
}

static void test_run_alu_and(void) {
    Word wc = int_to_word(0);

    wc.bits[CONTROL_BUS_SELALU_0_BIT_POSITION] = 0;
    wc.bits[CONTROL_BUS_SELALU_1_BIT_POSITION] = 1;
    wc.bits[CONTROL_BUS_SELALU_2_BIT_POSITION] = 0;

    wc.bits[CONTROL_BUS_ALUBUS_BIT_POSITION] = 1;  // release to databus

    publish_message_to(CONTROL_BUS_TOPIC, wc);

    Word A = int_to_word(1);
    Word B = int_to_word(1);
    publish_message_to(ACUMM_OUTPUT_BUS_TOPIC, A);
    publish_message_to(OP2_OUTPUT_BUS_TOPIC, B);

    run_alu();

    assert_int(word_to_int(data_bus->next_value), ==, word_to_int(A) & word_to_int(B));
    assert_int(word_to_int(alu_fc_output_bus->next_value), ==, 0);
    assert_int(word_to_int(alu_fz_output_bus->next_value), ==, 0);

    // ----

    A = int_to_word(0);
    B = int_to_word(1);
    publish_message_to(ACUMM_OUTPUT_BUS_TOPIC, A);
    publish_message_to(OP2_OUTPUT_BUS_TOPIC, B);

    run_alu();

    assert_int(word_to_int(data_bus->next_value), ==, word_to_int(A) & word_to_int(B));
    assert_int(word_to_int(alu_fc_output_bus->next_value), ==, 0);
    assert_int(word_to_int(alu_fz_output_bus->next_value), ==, 0);
}

static void test_run_alu_or(void) {
    Word wc = int_to_word(0);

    wc.bits[CONTROL_BUS_SELALU_0_BIT_POSITION] = 1;
    wc.bits[CONTROL_BUS_SELALU_1_BIT_POSITION] = 1;
    wc.bits[CONTROL_BUS_SELALU_2_BIT_POSITION] = 0;

    wc.bits[CONTROL_BUS_ALUBUS_BIT_POSITION] = 1;  // release to databus

    publish_message_to(CONTROL_BUS_TOPIC, wc);

    Word A = int_to_word(1);
    Word B = int_to_word(1);
    publish_message_to(ACUMM_OUTPUT_BUS_TOPIC, A);
    publish_message_to(OP2_OUTPUT_BUS_TOPIC, B);

    run_alu();

    assert_int(word_to_int(data_bus->next_value), ==, word_to_int(A) | word_to_int(B));
    assert_int(word_to_int(alu_fc_output_bus->next_value), ==, 0);
    assert_int(word_to_int(alu_fz_output_bus->next_value), ==, 0);

    // ----

    A = int_to_word(0);
    B = int_to_word(1);
    publish_message_to(ACUMM_OUTPUT_BUS_TOPIC, A);
    publish_message_to(OP2_OUTPUT_BUS_TOPIC, B);

    run_alu();

    assert_int(word_to_int(data_bus->next_value), ==, word_to_int(A) | word_to_int(B));
    assert_int(word_to_int(alu_fc_output_bus->next_value), ==, 0);
    assert_int(word_to_int(alu_fz_output_bus->next_value), ==, 0);
}

static void test_run_alu_xor(void) {
    Word wc = int_to_word(0);

    wc.bits[CONTROL_BUS_SELALU_0_BIT_POSITION] = 0;
    wc.bits[CONTROL_BUS_SELALU_1_BIT_POSITION] = 0;
    wc.bits[CONTROL_BUS_SELALU_2_BIT_POSITION] = 1;

    wc.bits[CONTROL_BUS_ALUBUS_BIT_POSITION] = 1;  // release to databus

    publish_message_to(CONTROL_BUS_TOPIC, wc);

    Word A = int_to_word(1);
    Word B = int_to_word(1);
    publish_message_to(ACUMM_OUTPUT_BUS_TOPIC, A);
    publish_message_to(OP2_OUTPUT_BUS_TOPIC, B);

    run_alu();

    assert_int(word_to_int(data_bus->next_value), ==, word_to_int(A) ^ word_to_int(B));
    assert_int(word_to_int(alu_fc_output_bus->next_value), ==, 0);
    assert_int(word_to_int(alu_fz_output_bus->next_value), ==, 1);

    // ----

    A = int_to_word(0);
    B = int_to_word(1);
    publish_message_to(ACUMM_OUTPUT_BUS_TOPIC, A);
    publish_message_to(OP2_OUTPUT_BUS_TOPIC, B);

    run_alu();

    assert_int(word_to_int(data_bus->next_value), ==, word_to_int(A) ^ word_to_int(B));
    assert_int(word_to_int(alu_fc_output_bus->next_value), ==, 0);
    assert_int(word_to_int(alu_fz_output_bus->next_value), ==, 0);
}

static void test_run_alu_not(void) {
    Word wc = int_to_word(0);

    wc.bits[CONTROL_BUS_SELALU_0_BIT_POSITION] = 1;
    wc.bits[CONTROL_BUS_SELALU_1_BIT_POSITION] = 0;
    wc.bits[CONTROL_BUS_SELALU_2_BIT_POSITION] = 1;

    wc.bits[CONTROL_BUS_ALUBUS_BIT_POSITION] = 1;  // release to databus

    publish_message_to(CONTROL_BUS_TOPIC, wc);

    Word A = int_to_word(1);
    Word B = int_to_word(1);
    publish_message_to(ACUMM_OUTPUT_BUS_TOPIC, A);
    publish_message_to(OP2_OUTPUT_BUS_TOPIC, B);

    run_alu();

    assert_int(word_to_int(data_bus->next_value), ==, 0xFE);
    assert_int(word_to_int(alu_fc_output_bus->next_value), ==, 0); // not does not set carry
    assert_int(word_to_int(alu_fz_output_bus->next_value), ==, 0);

    // ----

    A = int_to_word(0);
    B = int_to_word(1);
    publish_message_to(ACUMM_OUTPUT_BUS_TOPIC, A);
    publish_message_to(OP2_OUTPUT_BUS_TOPIC, B);

    run_alu();

    assert_int(word_to_int(data_bus->next_value), ==, 0xFF);
    assert_int(word_to_int(alu_fc_output_bus->next_value), ==, 0); // not does not set carry
    assert_int(word_to_int(alu_fz_output_bus->next_value), ==, 0);
}

static void test_run_alu_transparent(void) {
    Word wc = int_to_word(0);

    wc.bits[CONTROL_BUS_SELALU_0_BIT_POSITION] = 0;
    wc.bits[CONTROL_BUS_SELALU_1_BIT_POSITION] = 1;
    wc.bits[CONTROL_BUS_SELALU_2_BIT_POSITION] = 1;

    wc.bits[CONTROL_BUS_ALUBUS_BIT_POSITION] = 1;  // release to databus

    publish_message_to(CONTROL_BUS_TOPIC, wc);

    Word B = int_to_word(1);
    publish_message_to(OP2_OUTPUT_BUS_TOPIC, B);

    run_alu();

    assert_int(word_to_int(data_bus->next_value), ==, word_to_int(B));
    assert_int(word_to_int(alu_fc_output_bus->next_value), ==, 0);
    assert_int(word_to_int(alu_fz_output_bus->next_value), ==, 0);

    // ----

    B = int_to_word(5);
    publish_message_to(OP2_OUTPUT_BUS_TOPIC, B);

    run_alu();

    assert_int(word_to_int(data_bus->next_value), ==, word_to_int(B));
    assert_int(word_to_int(alu_fc_output_bus->next_value), ==, 0);
    assert_int(word_to_int(alu_fz_output_bus->next_value), ==, 0);
}

static void test_run_alu_increment(void) {
    Word wc = int_to_word(0);

    wc.bits[CONTROL_BUS_SELALU_0_BIT_POSITION] = 1;
    wc.bits[CONTROL_BUS_SELALU_1_BIT_POSITION] = 1;
    wc.bits[CONTROL_BUS_SELALU_2_BIT_POSITION] = 1;

    wc.bits[CONTROL_BUS_ALUBUS_BIT_POSITION] = 1;  // release to databus

    publish_message_to(CONTROL_BUS_TOPIC, wc);

    Word B = int_to_word(0xF);
    publish_message_to(OP2_OUTPUT_BUS_TOPIC, B);

    run_alu();

    assert_int(word_to_int(data_bus->next_value), ==, 0xF + 1);
    assert_int(word_to_int(alu_fc_output_bus->next_value), ==, 0);
    assert_int(word_to_int(alu_fz_output_bus->next_value), ==, 0);

    // ---- check overflow

    B = int_to_word(0xFF);
    publish_message_to(OP2_OUTPUT_BUS_TOPIC, B);

    run_alu();

    assert_int(word_to_int(data_bus->next_value), ==, 0x00);
    assert_int(word_to_int(alu_fc_output_bus->next_value), ==, 0); // increment does not set carry
    assert_int(word_to_int(alu_fz_output_bus->next_value), ==, 1);
}

static void teardown(void) {
    unsubscribe_for(acumm_output_bus_topic_subscription);
    unsubscribe_for(opt_output_bus_topic_subscription);
    unsubscribe_for(control_bus_topic_subscription);
    unsubscribe_for(data_bus_topic_subscription);
    unsubscribe_for(alu_fz_output_bus_subscription);
    unsubscribe_for(alu_fc_output_bus_subscription);

    destroy_bus_data(last_bus_acumm_output);
    destroy_bus_data(last_bus_op2_output);
    destroy_bus_data(control_bus);
    destroy_bus_data(data_bus);
    destroy_bus_data(alu_fc_output_bus);
    destroy_bus_data(alu_fz_output_bus);

    shutdown_alu();
}

int main(void) {
    log_set_quiet(1);

    setup();

    test_run_alu_sum();
    test_run_alu_sub();
    test_run_alu_or();
    test_run_alu_xor();
    test_run_alu_not();
    test_run_alu_transparent();
    test_run_alu_increment();

    teardown();
    return 0;
}
