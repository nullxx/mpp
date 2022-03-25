//
//  component.h
//  mpp-cpu
//
//  Created by Jon Lara trigo on 21/3/22.
//

#ifndef component_h
#define component_h

typedef struct {
    int value;
} LoadBit;

typedef struct {
    int bit_length;
    unsigned long long bin_value;
} Register;

typedef enum { NONE_EVENT_TYPE = 0, FLAG_CHANGE } CallbackEventType;

void init_components(void);
void shutdown_components(void);

#endif /* component_h */
