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
#include "acumm.h"
#include "addsub.h"
#include "alu.h"
#include "fffc.h"
#include "hl.h"
#include "mem.h"
#include "mxdir.h"
#include "pc.h"
#include "sp.h"

// TODO review the order of init and shutdown of components. Firt the ones that doesn't depend on any.

void init_components(void) {
    init_mem();
    init_mxdir();
    init_addsub();
    init_sp();
    init_pc();
    init_hl();
    init_fffc();
    init_alu();
    init_acumm();
}

void shutdown_components(void) {
    shutdown_mem();
    shutdown_mxdir();
    shutdown_addsub();
    shutdown_sp();
    shutdown_pc();
    shutdown_hl();
    shutdown_fffc();
    shutdown_alu();
    shutdown_acumm();
}
