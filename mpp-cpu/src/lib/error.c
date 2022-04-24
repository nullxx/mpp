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
        log_error(err.message == NULL ? "An error ocurred" : err.message);
    }
}

void process_error(Error err) {
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

void throw_error(Error err) {
    process_error(err);
}