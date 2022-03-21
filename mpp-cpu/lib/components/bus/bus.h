//
//  bus.h
//  mpp-cpu
//
//  Created by Jon Lara trigo on 21/3/22.
//

#ifndef bus_h
#define bus_h

typedef enum {
    NONE_BUS_BOUND_TYPE = 0,
    BUS,
    INPUT,
    OUTPUT
} BoundType;

typedef struct {
    BoundType type_start;
    BoundType type_endend;
} Bus;

#endif /* bus_h */
