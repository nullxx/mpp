//
//  utils.h
//  mpp-cpu
//
//  Created by Jon Lara trigo on 21/3/22.
//

#ifndef utils_h
#define utils_h
#include "vargs.h"

int hex_to_int(const char *hex);
char *int_to_hex(int num);

int random_int(const int min, const int max);
unsigned long long str_to_bin(const char *s);
int get_bin_len(unsigned long long bin);
char *bin_to_str(unsigned long long bin);
int bin_to_int(unsigned long long bin);
unsigned long long int_to_bin(int n, int max_len);

char *slice_str(const char *str, int start, int end);

char *create_str_internal(const int n, ...);
#define create_str(...) create_str_internal(VAR_COUNT(__VA_ARGS__), __VA_ARGS__)
char *initialize_str(char *str, int start, int end, char value);
char *str_concat(const char *str1, const char *str2);
char *str_dup(char *str);

char *itoa(int num);
unsigned concatenate(unsigned x, unsigned y);
void beep(void);
#endif /* utils_h */
