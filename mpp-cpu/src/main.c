//
//  main.c
//  mpp-cpu
//
//  Created by Jon Lara trigo on 21/3/22.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "lib/components/components.h"
#include "lib/logger.h"
#include "lib/utils.h"

void fn_exit(void) {
    log_info("Shuting down...");
    shutdown_components();
}

void on_error_fn(Error err) { log_error(err.message); }

int main(int argc, const char* argv[]) {
    (void)argc;
    (void)argv;

    log_info("Turning on...");

    jmp_buf error_buffer;
    init_err_hanlder(&error_buffer);
    ErrorType err_type = (ErrorType)setjmp(error_buffer);

    if (err_type != NONE_ERROR_TYPE) {
        process_error(NULL);
    } else {
        atexit(fn_exit);

        init_components();
    }

    return 0;
}
