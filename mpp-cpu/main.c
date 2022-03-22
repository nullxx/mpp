//
//  main.c
//  mpp-cpu
//
//  Created by Jon Lara trigo on 21/3/22.
//

#include <stdio.h>
#include "lib/utils.h"
#include "lib/components/components.h"
#include <stdlib.h>

void fnExit(void) {
	shutdown_components();
}

int main(int argc, const char * argv[]) {
	printf("Hello, World!\n");

	init_components();

	atexit(fnExit);
	return 0;
}
