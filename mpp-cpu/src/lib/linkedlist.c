#include "linkedlist.h"

#include <stdio.h>
#include <stdlib.h>

LlNode *create_ll_node(void *value, LlNode *next) {
    LlNode *l = (LlNode *)malloc(sizeof(LlNode));
    l->next = next;
    l->value = value;
    return l;
}

LlNode *push_ll_node(LlNode *head, void *val, LlNode *next) {
    LlNode *current = head;
    while (current->next != NULL) {
        current = current->next;
    }

    current->next = create_ll_node(val, next);

    return current->next;
}

get_node_t get_node_from_value(LlNode *head, void *value) {
    get_node_t t = {.prev_node = NULL, .node = NULL, .next_node = NULL, .found = 0};

    LlNode *current_node = head;

    while (current_node != NULL) {
        if (current_node->value == value) {
            break;
        }

        t.prev_node = current_node;
        current_node = current_node->next;
    }

    if (current_node == NULL) {
        t.found = 0;
        return t;
    }

    t.found = 1;
    t.node = current_node;
    t.next_node = current_node->next;

    return t;
}

/**
 * @brief Removes the reference from the list, but does not free node
 *
 * @param head
 * @param value
 * @return int
 */
int delete_node_from_value(LlNode **head, void *value) {
    get_node_t t = get_node_from_value(*head, value);
    if (!t.found) {
        return 0;
    }

    if (t.prev_node == NULL) {  // replace head
        *head = t.node->next;
    } else {  // prev to point to the next of current
        t.prev_node->next = t.node->next;
    }

    free(t.node);

    return 1;
}