//
//  error.c
//  mpp
//
//  Created by Jon Lara trigo on 19/3/22.
//

#include "error.h"

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

#include "logger.h"

jmp_buf err_buffer;
Error last_error;

void fatal(Error err) {
    if (err.show_errno == true) {
        perror(err.message);
    } else {
        log_fatal(err.message == NULL ? "An error ocurred" : err.message);
    }

    exit(EXIT_FAILURE);
}

void process_error(Error err) {
    switch (err.type) {
        case FATAL:
            fatal(err);
            break;
        default:
            break;
    }
}

void init_err_hanlder(void) {
    ErrorType err_type = (ErrorType) setjmp(err_buffer);

    if (err_type != NONE_ERROR_TYPE) {
        process_error(last_error);
        err_type = NONE_ERROR_TYPE;
    }
}

void throw_error(Error err) {
    last_error = err;
    longjmp(err_buffer, err.type);
}
