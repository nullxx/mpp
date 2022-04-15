/*
 * File: bus.h
 * Project: /src/lib/electronic/bus.h
 * File Created: Thursday, 24th March 2022 8:06:58 pm
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Thursday, 24th March 2022 8:07:00 pm
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */

#ifndef bus_h
#define bus_h
#include "../definitions.h"

typedef struct {
    Word next_value;
    Word current_value;
} Bus_t;

void init_buses(void);
void update_bus_data(Bus_t *bus_t);

Bus_t *create_bus_data(void);
void create_bus_data_from(Bus_t *bus_t);
void destroy_bus_data(Bus_t *bus_t);
#endif /* bus_h */