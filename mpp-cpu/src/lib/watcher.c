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

#include "linkedlist.h"
#include "logger.h"
#include "utils.h"

LlNode *watchers = NULL;

void register_watcher(RegisterWatcher *reg_watcher) {
    LlNode *node = create_ll_node(reg_watcher, NULL);
    if (watchers == NULL) {
        watchers = node;
    } else {
        push_ll_node(watchers, reg_watcher, NULL);
    }
}

void unregister_watcher(RegisterWatcher *reg_watcher) {
    if (watchers == NULL) {
        return;
    }
    
    delete_node_from_value(&watchers, reg_watcher);
}

void log_watchers(void) {
    for (LlNode *node = watchers; node != NULL; node = node->next) {
        RegisterWatcher *reg_watcher = (RegisterWatcher *) node->value;
        log_debug("%s: 0x%0X", reg_watcher->name, word_to_int(reg_watcher->reg->value));
    }
}