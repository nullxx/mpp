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

#include "lib/components/components.h"
#include "lib/components/cu/cu.h"
#include "lib/components/mem.h"
#include "lib/electronic/bus.h"
#include "lib/error.h"
#include "lib/logger.h"
#include "lib/watcher.h"

void on_signal_exit(int signal) {
    log_info("Received signal %d.", signal);
    printf("Do you want to exit? y/n: ");

    char confirmation = getchar();
    if (confirmation == 'y' || confirmation == '\n') {
        log_info("Shuting down...");
        shutdown_components();
        exit(EXIT_SUCCESS);
    }
}

void pause_execution(const char* message) {
    log_info(message);
    getchar();
}

void dispatch_clock_start(void) {
    // 1hz = 1 cyle per second
    // 1GHz = 1.000.000.000 cycles per second
    log_info("Starting clock...");
    jmp_buf error_buffer;
    init_err_hanlder(&error_buffer);

    log_info("Initial state");
    log_watchers();

    pause_execution("Press [ENTER] to start execution...");

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

        log_watchers();

        pause_execution("Press [ENTER] to continue...");
    }
}

int main(int argc, const char* argv[]) {
    (void)argc;
    (void)argv;

    log_info("Turning on...");

    signal(SIGINT, on_signal_exit);
    init_buses();
    init_components();

    // TODO remove

    // MOV 22, AC
    MemValue mem_value1 = {.offset = "0", .value_hex = "64"};
    set_mem_value(mem_value1);

    MemValue mem_value2 = {.offset = "1", .value_hex = "00"};
    set_mem_value(mem_value2);
    // -- MOV 22, AC

    // INC AC
    MemValue mem_value3 = {.offset = "2", .value_hex = "4B"};
    set_mem_value(mem_value3);
    // -- INC AC

    // // CMP 05
    // MemValue mem_value4 = {.offset = "3", .value_hex = "67"};
    // set_mem_value(mem_value4);

    // MemValue mem_value5 = {.offset = "4", .value_hex = "05"};
    // set_mem_value(mem_value5);
    // // -- CMP 05

    // JMP 0002
    MemValue mem_value9 = {.offset = "3", .value_hex = "74"};
    set_mem_value(mem_value9);

    MemValue mem_value10 = {.offset = "4", .value_hex = "00"};
    set_mem_value(mem_value10);

    MemValue mem_value11 = {.offset = "5", .value_hex = "02"};
    set_mem_value(mem_value11);
    // -- JMP 0002

    // FIN
    MemValue mem_value12 = {.offset = "6", .value_hex = "FF"};
    set_mem_value(mem_value12);
    // -- FIN

    // -- TODO remove
    dispatch_clock_start();

    return 0;
}
