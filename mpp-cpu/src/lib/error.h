//
//  error.h
//  mpp
//
//  Created by Jon Lara trigo on 19/3/22.
//

#ifndef error_h
#define error_h

typedef enum {
    FATAL,
} ErrorType;

typedef struct {
    ErrorType type;
    const char *message;
    unsigned int show_errno;
} Error;

void process_error(Error *err);
void fatal(Error *err);

#endif /* error_h */
