//
//  utils.h
//  mpp-cpu
//
//  Created by Jon Lara trigo on 21/3/22.
//

#ifndef utils_h
#define utils_h

int hex_to_int(const char *hex);
char *int_to_hex(int num);

int random_int(const int end_bound);
unsigned long long str_to_bin(const char *s);
int get_bin_len(unsigned long long bin);
char *bin_to_str(unsigned long long bin);
int bin_to_int(unsigned long long bin);
unsigned long long int_to_bin(int n);

#endif /* utils_h */
