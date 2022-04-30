#define MUNIT_ENABLE_ASSERT_ALIASES
#include "../../../../src/lib/components/addsub.h"

#include "../../../../src/lib/constants.h"
#include "../../../../src/lib/logger.h"
#include "../../../../src/lib/pubsub.h"
#include "../../../../src/lib/watcher.h"
#include "../../../_munit/munit.h"

static PubSubSubscription *dir_bus_topic1_subscription = NULL;
static PubSubSubscription *dir_bus_topic2_subscription = NULL;
static PubSubSubscription *control_bus_topic_subscription = NULL;
static Bus_t *control_bus;
static Bus_t *last_bus_dir1;
static Bus_t *last_bus_dir2;

static void setup(void) {
    last_bus_dir1 = create_bus_data();
    last_bus_dir2 = create_bus_data();
    control_bus = create_bus_data();
    dir_bus_topic1_subscription = subscribe_to(DIR_BUS_TOPIC_1, last_bus_dir1);
    dir_bus_topic2_subscription = subscribe_to(DIR_BUS_TOPIC_2, last_bus_dir2);
    control_bus_topic_subscription = subscribe_to(CONTROL_BUS_TOPIC, control_bus);

    init_addsub();
}

static void test_run_addsub(void) {
    Word wc;
    initialize_word(&wc, 0);

    wc.bits[CONTROL_BUS_ID_0_BIT_POSITION] = 0;
    wc.bits[CONTROL_BUS_ID_1_BIT_POSITION] = 1;

    publish_message_to(CONTROL_BUS_TOPIC, wc);

    Word wd = int_to_word(0);
    publish_message_to(DIR_BUS_TOPIC_2, wd);

    run_addsub();
    assert_int(word_to_int(last_bus_dir1->next_value), ==, word_to_int(wd) + 1);

    // ----

    initialize_word(&wc, 0);

    wc.bits[CONTROL_BUS_ID_0_BIT_POSITION] = 0;
    wc.bits[CONTROL_BUS_ID_1_BIT_POSITION] = 0;

    publish_message_to(CONTROL_BUS_TOPIC, wc);

    wd = int_to_word(0);
    publish_message_to(DIR_BUS_TOPIC_2, wd);

    run_addsub();
    assert_int(word_to_int(last_bus_dir1->next_value), ==, word_to_int(wd));

    // ----

    initialize_word(&wc, 0);

    wc.bits[CONTROL_BUS_ID_0_BIT_POSITION] = 1;
    wc.bits[CONTROL_BUS_ID_1_BIT_POSITION] = 0;

    publish_message_to(CONTROL_BUS_TOPIC, wc);

    wd = int_to_word(0);
    publish_message_to(DIR_BUS_TOPIC_2, wd);

    run_addsub();

    assert_int(word_to_int(last_bus_dir1->next_value), ==, word_to_int(wd));

    // ----

    initialize_word(&wc, 0);

    wc.bits[CONTROL_BUS_ID_0_BIT_POSITION] = 1;
    wc.bits[CONTROL_BUS_ID_1_BIT_POSITION] = 1;

    publish_message_to(CONTROL_BUS_TOPIC, wc);

    wd = int_to_word(1);
    publish_message_to(DIR_BUS_TOPIC_2, wd);

    run_addsub();
    assert_int(word_to_int(last_bus_dir1->next_value), ==, word_to_int(wd) - 1);
}

static void teardown(void) {
    unsubscribe_for(dir_bus_topic1_subscription);
    unsubscribe_for(dir_bus_topic2_subscription);
    unsubscribe_for(control_bus_topic_subscription);

    destroy_bus_data(last_bus_dir1);
    destroy_bus_data(last_bus_dir2);
    destroy_bus_data(control_bus);

    shutdown_addsub();
}

int main(void) {
    log_set_quiet(1);

    setup();

    test_run_addsub();

    teardown();
    return 0;
}
