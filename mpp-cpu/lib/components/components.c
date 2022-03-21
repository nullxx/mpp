//
//  component.c
//  mpp-cpu
//
//  Created by Jon Lara trigo on 21/3/22.
//

#include "components.h"
#include "mem.h"
#include <stdio.h>


void on_component_change_fn(CallbackEvent cbe) {
    return;
}

void init_components(void) {
    init_mem(&on_component_change_fn);
    MemValue v;
    v.offset = "0";
    v.value_hex = "FFFF";
    set_mem_value(v); // test
}
