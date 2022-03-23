//
//  component.c
//  mpp-cpu
//
//  Created by Jon Lara trigo on 21/3/22.
//
#include "components.h"

#include <stdio.h>

#include "addsub.h"
#include "mem.h"
#include "mxdir.h"

void init_components(void) {
    init_mem();
    init_mxdir();
    init_addsub();
}

void shutdown_components(void) { 
    shutdown_mem(); 
    shutdown_mxdir();
    shutdown_addsub();
}
