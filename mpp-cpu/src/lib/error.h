//
//  error.h
//  mpp
//
//  Created by Jon Lara trigo on 19/3/22.
//

#ifndef error_h
#define error_h

#include <setjmp.h>

typedef enum {
    NONE_ERROR_TYPE = 0,
    FATAL_ERROR,
    NOTICE_ERROR
} ErrorType;

typedef struct {
    ErrorType type;
    const char *message;
    unsigned int show_errno;
} Error;

void init_err_hanlder(jmp_buf *error_buffer);
void throw_error(Error err);

void process_error(void *error);
void fatal(Error err);
void notice(Error err);

#endif /* error_h */
