/*
 * File: /src/lib/watcher.h
 * Project: mpp-cpu
 * File Created: Thursday, 7th April 2022 4:24:29 pm
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Thursday, 7th April 2022 4:24:31 pm
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */

#ifndef watcher_h
#define watcher_h
#include "components/components.h"
typedef struct {
    Register *reg;
    char *name;
} RegisterWatcher;

void register_watcher(RegisterWatcher *reg);
void unregister_watcher(RegisterWatcher *reg_watcher);
void log_watchers(void);
#endif