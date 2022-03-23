//
//  main.c
//  mpp-cpu
//
//  Created by Jon Lara trigo on 21/3/22.
//

#include <stdio.h>
#include <stdlib.h>

#include "lib/components/components.h"
#include "lib/logger.h"
#include "lib/utils.h"

void fnExit(void) {
    log_info("Shuting down...");
    shutdown_components();
}

int main(int argc, const char* argv[]) {
    log_debug("Hello World!");

    atexit(fnExit);

    init_components();
    return 0;
}
