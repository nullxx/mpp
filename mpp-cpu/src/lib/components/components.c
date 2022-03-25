//
//  component.c
//  mpp-cpu
//
//  Created by Jon Lara trigo on 21/3/22.
//
#include "components.h"

#include <stdbool.h>
#include <stdio.h>

#include "../error.h"
#include "addsub.h"
#include "mem.h"
#include "mxdir.h"
#include "pc.h"
#include "sp.h"

void init_components(void) {
    init_mem();
    init_mxdir();
    init_addsub();
    init_sp();
    init_pc();
}

void shutdown_components(void) {
    shutdown_mem();
    shutdown_mxdir();
    shutdown_addsub();
    shutdown_sp();
    shutdown_pc();
}
