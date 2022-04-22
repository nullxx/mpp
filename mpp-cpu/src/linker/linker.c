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
#include <stdio.h>
#include "../lib/logger.h"
#include "../lib/components/components.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif


void (*update_ui)(void) = NULL;


#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void print_hello(void) {
    printf("Hello from linker\n");
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void init(void) {
    log_debug("Initializing components");
    init_components();
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void shutdown(void) {
    log_debug("Shuting down components");
    shutdown_components();
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void linker_set_update_ui(void (*update_ui_func)(void)) { update_ui = update_ui_func; }
