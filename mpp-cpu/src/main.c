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
#include "lib/utils.h"

#include "gui/gui.h"

jmp_buf error_buffer;

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

void init_error_handle(void) {
    ErrorType err_type = (ErrorType)setjmp(error_buffer);

    if (err_type != NONE_ERROR_TYPE) {
        process_error(NULL);
    }
}

int main(int argc, const char* argv[]) {
    (void)argc;
    (void)argv;

    // start_gui(argc, argv);

    log_info("Turning on...");

    signal(SIGINT, on_signal_exit);

    init_err_hanlder(&error_buffer);
    init_error_handle();

    init_buses();
    init_components();

    // TODO remove

    // MOV 22, AC
    MemValue mem_value1 = {.offset = 0x00, .value_hex = "64"};
    set_mem_value(mem_value1);

    MemValue mem_value2 = {.offset = 0x01, .value_hex = "02"};
    set_mem_value(mem_value2);
    // -- MOV 22, AC

    // INC AC
    MemValue mem_value3 = {.offset = 0x02, .value_hex = "45"};
    set_mem_value(mem_value3);
    // -- INC AC

    // // CMP 05
    // MemValue mem_value4 = {.offset = 0x03, .value_hex = "40"};
    // set_mem_value(mem_value4);

    // MemValue mem_value5 = {.offset = 0x04, .value_hex = "30"};
    // set_mem_value(mem_value5);
    // // -- CMP 05

    // CMP 05
    // MemValue mem_value6 = {.offset = 0x05, .value_hex = "30"};
    // set_mem_value(mem_value6);

    // MemValue mem_value7 = {.offset = 0x06, .value_hex = "00"};
    // set_mem_value(mem_value7);

    // MemValue mem_value8 = {.offset = 0x07, .value_hex = "0B"};
    // set_mem_value(mem_value8);
    // // -- CMP 05

    // // JMP 0002
    // MemValue mem_value9 = {.offset = 0x08, .value_hex = "74"};
    // set_mem_value(mem_value9);

    // MemValue mem_value10 = {.offset = 0x09, .value_hex = "00"};
    // set_mem_value(mem_value10);

    // MemValue mem_value11 = {.offset = 0x0A, .value_hex = "02"};
    // set_mem_value(mem_value11);
    // // -- JMP 0002

    // // FIN
    // MemValue mem_value12 = {.offset = 0x0B, .value_hex = "FF"};
    // set_mem_value(mem_value12);
    // // -- FIN

    init_cu(); // temporal solution for loading instructions
    // -- TODO remove
    return 0;
}
