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
#include "op2.h"
#include "mxreg.h"
#include "greg.h"
#include "controllers/mxfldx.h"
#include "controllers/flagsinta.h"
#include "flags.h"
#include "cu/cu.h"
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
    init_op2();
    init_mxreg();
    init_greg();
    cll_init_mxfldx();
    init_flags();
    cll_init_flagsinta();
    init_cu();
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
    shutdown_op2();
    shutdown_mxreg();
    shutdown_greg();
    cll_shutdown_mxfldx();
    shutdown_flags();
    cll_shutdown_flagsinta();
    shutdown_cu();
}
