/*
 * File: /src/lib/electronic/and.c
 * Project: mpp-cpu
 * File Created: Monday, 18th April 2022 7:12:47 pm
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Monday, 18th April 2022 7:12:55 pm
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */

#include "and.h"

int AND(int numbers[], int size) {
    int result = 1;
    for (int i = 0; i < size; i++) {
        result = result & numbers[i];
    }

    return result;
}

int OR(int numbers[], int size) {
    int result = 0;
    for (int i = 0; i < size; i++) {
        result = result | numbers[i];
    }

    return result;
}