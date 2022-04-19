//
//  main.c
//  mpp-cpu
//
//  Created by Jon Lara trigo on 21/3/22.
//

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "gui/gui.h"
#include "lib/components/components.h"
#include "lib/components/cu/cu.h"
#include "lib/components/mem.h"
#include "lib/electronic/bus.h"
#include "lib/error.h"
#include "lib/logger.h"
#include "lib/watcher.h"
#include "linker.h"

jmp_buf error_buffer;

void on_signal_exit(int signal) {
    log_info("Received signal %d.", signal);
    shutdown_components();
    exit(EXIT_SUCCESS);
}

void init_error_handle(void) { // TODO check error handling
    ErrorType err_type = (ErrorType)setjmp(error_buffer);

    if (err_type != NONE_ERROR_TYPE) {
        process_error(NULL);
    }
}

int main(int argc, const char* argv[]) {
    (void)argc;
    (void)argv;

    log_info("Turning on...");

    signal(SIGINT, on_signal_exit);

    init_err_hanlder(&error_buffer);
    init_error_handle();

    init_buses();
    init_components();

    init_linker();

    init_gui();

    shutdown_linker();
    shutdown_components();

    return 0;
}
