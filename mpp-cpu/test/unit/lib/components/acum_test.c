#define MUNIT_ENABLE_ASSERT_ALIASES
#include "../../../../src/lib/components/acumm.h"
#include "../../../../src/lib/constants.h"
#include "../../../../src/lib/pubsub.h"
#include "../../../../src/lib/watcher.h"
#include "../../../../src/lib/logger.h"
#include "../../../_munit/munit.h"

PubSubSubscription *acum_output_bus_topic;
PubSubSubscription *data_bus_topic;
Bus_t *acum_output_bus;
Bus_t *data_bus_topic_bus;

static void setup(void) {
    acum_output_bus = create_bus_data();
    data_bus_topic_bus = create_bus_data();

    acum_output_bus_topic = subscribe_to(ACUMM_OUTPUT_BUS_TOPIC, acum_output_bus);
    data_bus_topic = subscribe_to(DATA_BUS_TOPIC, data_bus_topic_bus);

    init_acumm();
}

static void test_run_acumm(void) {
    Word wc;
    initialize_word(&wc, 0);
    wc.bits[CONTROL_BUS_ACCAR_BIT_POSITION] = 1;
    publish_message_to(CONTROL_BUS_TOPIC, wc);

    Word wd = int_to_word(0xFF);
    publish_message_to(DATA_BUS_TOPIC, wd);

    run_acumm();

    Register *r = get_register(WATCHER_TYPE_ACUM);
    assert_not_null(r);
    assert_int(word_to_int(r->value), ==, 0xFF);
    assert_int(word_to_int(acum_output_bus->next_value), ==, word_to_int(r->value));

    // ----
    
    Word last_reg_value = r->value;
    initialize_word(&wc, 0);
    wc.bits[CONTROL_BUS_ACCAR_BIT_POSITION] = 1;
    publish_message_to(CONTROL_BUS_TOPIC, wc);

    int num = 0xFFF;
    assert_int(num, >, r->bit_length);
    wd = int_to_word(num);
    
    publish_message_to(DATA_BUS_TOPIC, wd);

    run_acumm();

    assert_int(word_to_int(r->value), ==, word_to_int(last_reg_value));
    assert_int(word_to_int(acum_output_bus->next_value), ==, word_to_int(r->value));
    
    // ----

    initialize_word(&wc, 0);
    wc.bits[CONTROL_BUS_ACBUS_BIT_POSITION] = 1;
    publish_message_to(CONTROL_BUS_TOPIC, wc);

    run_acumm();

    assert_int(word_to_int(data_bus_topic_bus->next_value), ==, word_to_int(r->value));
    assert_int(word_to_int(acum_output_bus->next_value), ==, word_to_int(r->value));
}

static void teardown(void) {
    unsubscribe_for(acum_output_bus_topic);
    unsubscribe_for(data_bus_topic);

    destroy_bus_data(acum_output_bus);
    destroy_bus_data(data_bus_topic_bus);
    shutdown_acumm();
}

int main(void) {
    log_set_quiet(1);

    setup();

    test_run_acumm();

    teardown();
    return 0;
}
