/*
 * File: /src/lib/utils.h
 * Project: mpp-cpu
 * File Created: Friday, 15th April 2022 1:23:58 pm
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Saturday, 23rd April 2022 2:11:30 am
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */

#ifndef utils_h
#define utils_h

int random_int(const int min, const int max);
int get_used_bits(int number);

char *num_to_str(int num);
int get_bit(int n, int k);
#endif /* utils_h */
