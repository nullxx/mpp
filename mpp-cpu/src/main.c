//
//  main.c
//  mpp-cpu
//
//  Created by Jon Lara trigo on 21/3/22.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "lib/components/components.h"
#include "lib/components/cu/cu.h"
#include "lib/electronic/bus.h"
#include "lib/error.h"
#include "lib/logger.h"

static void fn_exit(void) {
    log_info("Shuting down...");
    shutdown_components();
}

void pause_execution(void) {
    log_info("Press enter to continue...");
    getchar();
}

void dispatch_clock_start(void) {
    // 1hz = 1 cyle per second
    // 1GHz = 1.000.000.000 cycles per second
    log_info("Starting clock...");
    jmp_buf error_buffer;
    init_err_hanlder(&error_buffer);
    while (1) {
        ErrorType err_type = (ErrorType)setjmp(error_buffer);

        if (err_type != NONE_ERROR_TYPE) {
            process_error(NULL);
        }

        clock_t start = clock();
        run_cu();
        clock_t end = clock();

        double seconds_spent = (double)(end - start) / CLOCKS_PER_SEC;
        log_info("Cycle time: %fs => %f KHz", seconds_spent, (1 / seconds_spent) / 1000);
        
        pause_execution();
    }
}

int main(int argc, const char* argv[]) {
    (void)argc;
    (void)argv;

    log_info("Turning on...");

    atexit(fn_exit);

    init_buses();
    init_components();
    dispatch_clock_start();

    return 0;
}
