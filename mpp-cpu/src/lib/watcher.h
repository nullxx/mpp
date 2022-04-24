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

typedef enum {
    WATCHER_TYPE_ACUM,
    WATCHER_TYPE_FC,
    WATCHER_TYPE_FZ,
    WATCHER_TYPE_B,
    WATCHER_TYPE_C,
    WATCHER_TYPE_D,
    WATCHER_TYPE_E,
    WATCHER_TYPE_H,
    WATCHER_TYPE_L,
    WATCHER_TYPE_2OP,
    WATCHER_TYPE_PCH,
    WATCHER_TYPE_PCL,
    WATCHER_TYPE_PC,
    WATCHER_TYPE_SP,
    WATCHER_TYPE_RI
} WatcherType;

typedef struct {
    Register* reg;
    WatcherType type;
} RegisterWatcher;

void register_watcher(RegisterWatcher* reg);
void unregister_watcher(RegisterWatcher* reg_watcher);

Register* get_register(WatcherType type);
#endif