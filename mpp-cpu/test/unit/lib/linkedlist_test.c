#define MUNIT_ENABLE_ASSERT_ALIASES
#include "../../../src/lib/linkedlist.h"

#include "../../_munit/munit.h"

void test_create_ll_node(void) {
    int value = 0;
    LlNode *node = create_ll_node(&value, NULL);
    assert_not_null(node);
    assert_null(node->next);
    assert_ptr_equal(node->value, &value);
}

void test_push_ll_node(void) {
    int value = 0;
    LlNode *node = create_ll_node(&value, NULL);

    int value2 = 1;
    LlNode *next_node = push_ll_node(node, &value2, NULL);

    assert_ptr_equal(node->next, next_node);
    assert_ptr_equal(next_node->value, &value2);
    assert_null(next_node->next);
}

void test_get_node_from_value(void) {
    int value = 0;
    LlNode *node = create_ll_node(&value, NULL);

    int value2 = 1;
    LlNode *next_node = push_ll_node(node, &value2, NULL);

    get_node_t gn1 = get_node_from_value(node, &value);
    get_node_t gn2 = get_node_from_value(node, &value2);

    assert_ptr_equal(gn1.prev_node, NULL);  // assert_ptr_null not working
    assert_ptr_equal(gn1.node, node);
    assert_ptr_equal(gn1.node, gn2.prev_node);
    assert_ptr_equal(gn1.next_node, node->next);
    assert_ptr_equal(gn1.next_node, gn2.node);

    assert_ptr_equal(gn2.prev_node, node);
    assert_ptr_equal(gn2.node, next_node);
    assert_ptr_equal(gn2.next_node, NULL);  // assert_ptr_null not working
}

void test_delete_node_from_value(void) {
    int value = 0;
    LlNode *node = create_ll_node(&value, NULL);

    int value2 = 1;
    push_ll_node(node, &value2, NULL);

    int deleted = delete_node_from_value(&node, &value2);

    assert_true(deleted);
    assert_ptr_equal(node->next, NULL);
}

int main(void) {
    test_create_ll_node();
    test_push_ll_node();
    test_get_node_from_value();
    test_delete_node_from_value();

    return 0;
}
