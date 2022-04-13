/*
 * File: /src/lib/definitions.h
 * Project: mpp-cpu
 * File Created: Sunday, 10th April 2022 10:24:59 pm
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Sunday, 10th April 2022 10:25:01 pm
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */

#ifndef definitions_h
#define definitions_h

typedef unsigned long long Bin;
typedef struct {
    Bin next_value;
    Bin current_value;
} Bus_t;

#define BUS_SIZE_BIT 64
#define BUS_DEFAULT_VALUE 0xFFFFFFFFFFFFFFFF;
#endif