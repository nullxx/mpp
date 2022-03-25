//
//  main.c
//  mpp-cpu
//
//  Created by Jon Lara trigo on 21/3/22.
//

#include <stdio.h>
#include <stdlib.h>

#include "lib/components/components.h"
#include "lib/electronic/bus.h"
#include "lib/logger.h"
#include "lib/utils.h"
#include "lib/error.h"

static void fn_exit(void) {
    log_info("Shuting down...");
    shutdown_components();
}

int main(int argc, const char* argv[]) {
    (void)argc;
    (void)argv;

    log_info("Turning on...");

    atexit(fn_exit);

    // THIS IS TEMPORAL HERE UNTIL unit control is coded
    jmp_buf error_buffer;
    init_err_hanlder(&error_buffer);
    ErrorType err_type = (ErrorType)setjmp(error_buffer);

    if (err_type != NONE_ERROR_TYPE) {
        process_error(NULL);
    } else {
        init_buses();
        init_components();
    }
    // -- THIS IS TEMPORAL HERE UNTIL unit control is coded

    return 0;
}
