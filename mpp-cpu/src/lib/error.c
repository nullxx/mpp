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

#include "logger.h"

jmp_buf *err_buffer;
Error last_error;

void fatal(Error err) {
    if (err.show_errno == true) {
        perror(err.message);
    } else {
        log_fatal(err.message == NULL ? "An error ocurred" : err.message);
    }

    exit(EXIT_FAILURE);
}

void notice(Error err) {
    if (err.show_errno == true) {
        perror(err.message);
    } else {
        log_fatal(err.message == NULL ? "An error ocurred" : err.message);
    }
}

void process_error(void *error) {
    Error err;
    if (error == NULL) {
        err = last_error;
    } else {
        err = *(Error *) error;
    }
    
    switch (err.type) {
        case FATAL_ERROR:
            fatal(err);
            break;
        
        case NOTICE_ERROR:
            notice(err);
            break;
        default:
            break;
    }
}

void init_err_hanlder(jmp_buf *error_buffer) {
    err_buffer = error_buffer;
}

void throw_error(Error err) {
    last_error = err;
    longjmp(err_buffer, err.type);
}
