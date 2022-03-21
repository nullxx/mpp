//
//  main.c
//  mpp-cpu
//
//  Created by Jon Lara trigo on 21/3/22.
//

#include <stdio.h>
#include "lib/utils.h"
#include "lib/components/components.h"

int main(int argc, const char * argv[]) {
    // insert code here...
    printf("Hello, World!\n");
    
    char *hex = int_to_hex(999999999);
    
    printf("Hello, World! %s\n", hex);
    init_components();
    return 0;
}
