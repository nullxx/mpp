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

void fatal(Error *err) {
    if (err->show_errno == true) {
        perror(err->message);
    } else {
        fprintf(stderr, "%s", err->message == NULL ? "An error ocurred" : err->message);
    }

    exit(EXIT_FAILURE);
}

void process_error(Error *err) {
    switch (err->type) {
        case FATAL:
            fatal(err);
            break;
        default:
            break;
    }
}
