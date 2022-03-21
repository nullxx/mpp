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

typedef enum {
    NONE_FLAG_TYPE = 0,
    INPUT,
    OUTPUT
} FlagType;

typedef struct {
    bool internal;
    FlagType type;
    int value;
} Flag;

typedef enum {
    NONE_EVENT_TYPE = 0,
    FLAG_CHANGE
} CallbackEventType;

typedef  struct {
    CallbackEventType type;
    Flag flag;
} CallbackEvent;

typedef void (*on_component_change)(CallbackEvent);

void init_components(void);
void step(void);

#endif /* component_h */
