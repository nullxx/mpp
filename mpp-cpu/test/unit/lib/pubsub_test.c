#define MUNIT_ENABLE_ASSERT_ALIASES
#include "../../../src/lib/pubsub.h"

#include <stdio.h>

#include "../../_munit/munit.h"

static int fn1_used = -1;
static int fn2_used = -1;

PubSubSubscription *test_subscribe_to(Bus_t *bus) {
    PubSubSubscription *sub = subscribe_to(DATA_BUS_TOPIC, bus);

    assert_ptr_not_null(sub);
    assert_true(sub->active);
    assert_ptr_not_null(sub->bus_t);
    assert_int(sub->id, >=, 0);
    assert(sub->topic == DATA_BUS_TOPIC);

    return sub;
}

void test_unsubscribe_for(PubSubSubscription *sub) {
    assert_ptr_not_null(sub);

    int success = unsubscribe_for(sub);

    assert_true(success);
    assert_false(sub->active);
}

void test_publish_message_to(Bus_t *bus) {
    Word w;
    initialize_word(&w, 0);

    int success = publish_message_to(DATA_BUS_TOPIC, w);

    assert_true(success);

    assert_int(word_to_int(w), ==, word_to_int(bus->next_value));

    w.bits[0] = 1;
    w.bits[1] = 1;
    w.bits[2] = 1;
    w.bits[3] = 1;

    success = publish_message_to(DATA_BUS_TOPIC, w);

    assert_true(success);
    assert_int(word_to_int(w), ==, word_to_int(bus->next_value));
}

static int test_add_topic_middleware_fn1(Word value) {
    fn1_used = 1;
    return 1;
}

PubSubMiddleware *test_add_topic_middleware1(void) {
    PubSubMiddleware *mid = add_topic_middleware(DATA_BUS_TOPIC, test_add_topic_middleware_fn1);

    assert_ptr_not_null(mid);
    assert(mid->id >= 0);
    assert(mid->middlware == test_add_topic_middleware_fn1);
    assert(mid->topic == DATA_BUS_TOPIC);

    Word w;
    initialize_word(&w, 0);

    int success = publish_message_to(DATA_BUS_TOPIC, w);

    assert_true(success);
    assert_true(fn1_used);
    return mid;
}

static int test_add_topic_middleware_fn2(Word value) {
    fn2_used = 1;
    return 0;
}

PubSubMiddleware *test_add_topic_middleware2(void) {
    PubSubMiddleware *mid = add_topic_middleware(DATA_BUS_TOPIC, test_add_topic_middleware_fn2);

    assert_ptr_not_null(mid);
    assert(mid->id >= 0);
    assert(mid->middlware == test_add_topic_middleware_fn2);
    assert(mid->topic == DATA_BUS_TOPIC);

    Word w;
    initialize_word(&w, 0);

    int success = publish_message_to(DATA_BUS_TOPIC, w);

    assert_int(success, ==, -1);
    assert_true(fn2_used);
    return mid;
}

void test_rm_topic_middleware(PubSubMiddleware *mid1, PubSubMiddleware *mid2) {
    int sccs1 = rm_topic_middleware(mid1);
    int sccs2 = rm_topic_middleware(mid2);

    assert_true(sccs1);
    assert_true(sccs2);

    Word w;
    initialize_word(&w, 0);

    int success = publish_message_to(DATA_BUS_TOPIC, w);

    assert_true(success);
}

int main(void) {
    Bus_t *bus = create_bus_data();

    PubSubSubscription *sub = test_subscribe_to(bus);
    test_publish_message_to(bus);
    PubSubMiddleware *mid1 = test_add_topic_middleware1();
    PubSubMiddleware *mid2 = test_add_topic_middleware2();
    test_rm_topic_middleware(mid1, mid2);
    test_unsubscribe_for(sub);

    destroy_bus_data(bus);

    return 0;
}
