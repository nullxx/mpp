//
//  utils.h
//  mpp-cpu
//
//  Created by Jon Lara trigo on 21/3/22.
//

#ifndef utils_h
#define utils_h

#include <stdio.h>

int hex_to_int(const char *hex);
char *int_to_hex(int num);

int random_int(const int end_bound);

#endif /* utils_h */
