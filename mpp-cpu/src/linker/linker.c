/*
 * File: /src/linker.c
 * Project: mpp-cpu
 * File Created: Saturday, 16th April 2022 8:43:06 pm
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Saturday, 16th April 2022 8:43:10 pm
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */

#include "linker.h"
#include "cu.h"
#include <stdio.h>

#include "../lib/components/components.h"
#include "../lib/logger.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

update_ui_fn update_ui = NULL;

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void print_hello(void) { printf("Hello from linker\n"); }

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void init(void) {
    log_debug("Initializing linker");
    init_components();
    init_linker_cu();
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void shutdown(void) {
    log_debug("Shuting down linker");
    shutdown_linker_cu();
    shutdown_components();
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void linker_set_update_ui(void (*update_ui_func)(void)) {
    update_ui = update_ui_func;
    log_debug("Set update ui function");
}

update_ui_fn get_update_ui_fn(void) { return update_ui; }