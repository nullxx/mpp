//
//  error.c
//  mpp
//
//  Created by Jon Lara trigo on 19/3/22.
//

#include "error.h"
#include "logger.h"

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>

void fatal(Error *err) {
    if (err->show_errno == true) {
        perror(err->message);
    } else {
        log_error(err->message == NULL ? "An error ocurred" : err->message);
    }
    
end:
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
