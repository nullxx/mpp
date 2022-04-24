/*
 * File: /src/linker/registers.c
 * Project: mpp-cpu
 * File Created: Friday, 22nd April 2022 5:18:42 pm
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Friday, 22nd April 2022 5:18:45 pm
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "../lib/components/components.h"
#include "../lib/logger.h"
#include "../lib/watcher.h"
#include "linker.h"

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
int get_register_acum(void) { return word_to_int(get_register(WATCHER_TYPE_ACUM)->value); }

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
int get_register_fc(void) { return word_to_int(get_register(WATCHER_TYPE_FC)->value); }

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
int get_register_fz(void) { return word_to_int(get_register(WATCHER_TYPE_FZ)->value); }

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
int get_register_b(void) { return word_to_int(get_register(WATCHER_TYPE_B)->value); }

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
int get_register_c(void) { return word_to_int(get_register(WATCHER_TYPE_C)->value); }

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
int get_register_d(void) { return word_to_int(get_register(WATCHER_TYPE_D)->value); }

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
int get_register_e(void) { return word_to_int(get_register(WATCHER_TYPE_E)->value); }

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
int get_register_h(void) { return word_to_int(get_register(WATCHER_TYPE_H)->value); }

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
int get_register_l(void) { return word_to_int(get_register(WATCHER_TYPE_L)->value); }

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
int get_register_2op(void) { return word_to_int(get_register(WATCHER_TYPE_2OP)->value); }

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
int get_register_pch(void) { return word_to_int(get_register(WATCHER_TYPE_PCH)->value); }

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
int get_register_pcl(void) { return word_to_int(get_register(WATCHER_TYPE_PCL)->value); }

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
int get_register_pc(void) { return word_to_int(get_register(WATCHER_TYPE_PC)->value); }

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
int get_register_sp(void) { return word_to_int(get_register(WATCHER_TYPE_SP)->value); }

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
int get_register_ri(void) { return word_to_int(get_register(WATCHER_TYPE_RI)->value); }

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void set_register_pc(int value) {
    get_register(WATCHER_TYPE_PC)->value = int_to_word(value);
    get_update_ui_fn()();
}