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
    log_info("Turning on...");

    atexit(fnExit);

    init_components();

    int len = get_bin_len(1111001111111111);
    log_debug("%d bits", len);
    
    return 0;
}
