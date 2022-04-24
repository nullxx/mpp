//
//  component.h
//  mpp-cpu
//
//  Created by Jon Lara trigo on 21/3/22.
//

#ifndef component_h
#define component_h
#include "../definitions.h"
typedef struct {
    int value;
} LoadBit;

typedef struct {
    int bit_length;
    Word value;
} Register;

void init_components(void);
void shutdown_components(void);

#endif /* component_h */
