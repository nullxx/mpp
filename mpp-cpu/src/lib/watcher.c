/*
 * File: /src/lib/watcher.c
 * Project: mpp-cpu
 * File Created: Thursday, 7th April 2022 4:24:11 pm
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Thursday, 7th April 2022 4:24:14 pm
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */

#include "watcher.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linkedlist.h"
#include "logger.h"
#include "utils.h"

LlNode *head;
void register_watcher(RegisterWatcher *reg_watcher) {
    LlNode *node = create_ll_node(reg_watcher, NULL);

    if (head == NULL) {
        head = node;
    } else {
        LlNode *current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = node;
    }
}

void unregister_watcher(RegisterWatcher *reg_watcher) {
    if (head == NULL) return;
    delete_node_from_value(&head, reg_watcher);
}

Register *get_register(WatcherType type) {
    LlNode *current = head;
    while (current != NULL) {
        RegisterWatcher *reg_watcher = (RegisterWatcher *)current->value;
        if (reg_watcher->type == type) {
            return reg_watcher->reg;
        }
        current = current->next;
    }
    return NULL;
}