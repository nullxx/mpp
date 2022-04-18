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
    Register* reg;
    const char* name;
    unsigned int changed;
    char* hex_repr;
} RegisterWatcher;

typedef struct {
    RegisterWatcher* PC;
    RegisterWatcher* PCH;
    RegisterWatcher* PCL;
    RegisterWatcher* H;
    RegisterWatcher* L;
    RegisterWatcher* SP;
    RegisterWatcher* FC;
    RegisterWatcher* FZ;
    RegisterWatcher* B;
    RegisterWatcher* C;
    RegisterWatcher* D;
    RegisterWatcher* E;
    RegisterWatcher* RI;
    RegisterWatcher* ACUM;
    RegisterWatcher* OP2;
} Watchers;

void register_watcher(RegisterWatcher* reg);
void unregister_watcher(RegisterWatcher* reg_watcher);
Watchers get_watchers(int raw);
#endif