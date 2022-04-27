//
//  utils.c
//  mpp-cpu
//
//  Created by Jon Lara trigo on 21/3/22.
//

#include "utils.h"

#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_used_bits(int number) { return log2(number) + 1; }
int get_num_digits(int number) {
    int count = 0;
    while (number != 0) {
        number = number / 10;
        count++;
    }

    return count;
}

int random_int(const int min, const int max) {
    int r = rand() % max + min;
    return r;
}

char *num_to_str(int num) {
    const int num_len = (int)log10(num + 1) + 1 + 1;  // last +1 for the null terminator
    char *str = (char *)malloc(sizeof(char) * num_len + 1);
    snprintf(str, sizeof(char) * num_len, "%d", num);
    return str;
}

int get_bit(int n, int k) {
    int mask = 1 << k;
    int masked_n = n & mask;
    int thebit = masked_n >> k;
    return thebit;
}