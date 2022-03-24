//
//  error.h
//  mpp
//
//  Created by Jon Lara trigo on 19/3/22.
//

#ifndef error_h
#define error_h

typedef enum {
    NONE_ERROR_TYPE = 0,
    FATAL,
} ErrorType;

typedef struct {
    ErrorType type;
    const char *message;
    unsigned int show_errno;
} Error;

// void process_error(Error *err);
void init_err_hanlder(void);
void throw_error(Error err);
void fatal(Error err);

#endif /* error_h */
