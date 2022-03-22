//
//  component.h
//  mpp-cpu
//
//  Created by Jon Lara trigo on 21/3/22.
//

#ifndef component_h
#define component_h

#include <stdbool.h>
#include "../error.h"

typedef  struct {
    bool success;
    Error err;
    void *return_value;
} ComponentActionReturn;

typedef struct {
    int value;
} LoadBit;

typedef enum {
    NONE_EVENT_TYPE = 0,
    FLAG_CHANGE
} CallbackEventType;

void init_components(void);
void shutdown_components(void);

#endif /* component_h */
