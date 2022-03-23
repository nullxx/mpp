//
//  component.c
//  mpp-cpu
//
//  Created by Jon Lara trigo on 21/3/22.
//
#include "components.h"

#include <stdio.h>

#include "mem.h"

void init_components(void) { init_mem(); }

void shutdown_components(void) { shutdown_mem(); }
