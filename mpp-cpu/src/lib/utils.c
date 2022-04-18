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

int hex_to_dec(const char *hex) {
    int number = (int)strtol(hex, NULL, 16);
    return number;
}

int get_used_bits(int number) { return log2(number) + 1; }
int get_num_digits(int number) {
    int count = 1;
    while (number != 0) {
        number = number / 10;
        count++;
    }

    return count;
}
char *int_to_hex(const char *prefix, int num) {
    int size = get_num_digits(num) + strlen(prefix) + 1;
    char *result = (char *)malloc(sizeof(char) * size);
    if (result == NULL) {
        return NULL;
    }

    snprintf(result, size, "%s%X", prefix, num);
    return result;
}

int random_int(const int min, const int max) {
    int r = min + rand() % (max - min);
    return r;
}

unsigned long long str_to_bin(const char *s) {
    const int len = strlen(s);
    unsigned long long result = 0;
    for (int i = 0; i < len; i++) {
        if (s[i] == '0') {
            result = concatenate(result, 0);
        } else if (s[i] == '1') {
            result = concatenate(result, 1);
        }
    }
    return result;
}

char *bin_to_str(unsigned long long bin) {
    const int bin_len = get_used_bits(bin);

    const size_t size = sizeof(char) * (bin_len + 1);
    char *str = (char *)malloc(size);

    snprintf(str, size, "%llu", bin);
    return str;
}

int bin_to_dec(unsigned long long num) {
    int i = 0, decimal = 0;

    // converting binary to decimal
    while (num != 0) {
        int digit = num % 10;
        decimal += digit * pow(2, i);

        num /= 10;
        i++;
    }
    return decimal;
}
// int bin_to_dec(unsigned long long bin) {
//     int result = 0;
//     int bin_len = get_num_len(bin);
//     for (int i = 0; i < bin_len; i++) {
//         const int bit = (int)pow(2, i);
//         if (bin & bit) {
//             result += bit;
//         }
//     }
//     return result;
// }

// unsigned long long int_to_bin(int n) {
//     long long bin = 0;
//     int rem, i = 1;

//     while (n != 0) {
//         rem = n % 2;
//         n /= 2;
//         bin += rem * i;
//         i *= 10;
//     }

//     return bin;
// }
// unsigned long long int_to_bin(unsigned k) {
//     if (k == 0) return 0;
//     if (k == 1) return 1;                       /* optional */
//     return (k % 2) + 10 * int_to_bin(k / 2);
// }

unsigned long long int_to_bin(int n, const int max_len) {
    unsigned long long result = 0;

    for (int c = max_len - 1; c >= 0; c--) {
        int d = n >> c;
        if (d & 1) {
            result = concatenate(result, 1);
        } else {
            result = concatenate(result, 0);
        }
    }

    return result;
}

char *slice_str(const char *str, int start, int end) {
    char *result = (char *)malloc(sizeof(char) * (end - start) + 1);
    int j = 0;

    for (int i = start; i <= end; ++i) {
        result[j++] = str[i];
    }

    result[j] = 0;

    return result;
}

char *initialize_str(char *str, int start, int end, char value) {
    if (end <= start) return NULL;

    for (int i = start; i <= end; i++) {
        str[i] = value;
    }

    return str;
}

char *create_str_internal(const int n, ...) {
    va_list lptr;
    va_start(lptr, n);
    char *str = NULL;

    for (int i = 0; i < n; i++) {
        const int _first = str == NULL;
        const char *chunk = va_arg(lptr, char *);

        const int prev_size = _first ? 0 : strlen(str);
        const int next_size = sizeof(char) * (strlen(chunk) + strlen(" ") + prev_size + 1);

        str = realloc(str, next_size);

        if (_first) {
            if (str == NULL) return NULL;

            initialize_str(str, 0, next_size, 0);
        } else {
            if (str == NULL) return NULL;

            initialize_str(str, prev_size, next_size, 0);
            strcat(str, " ");
        }
        strcat(str, chunk);
    }
    va_end(lptr);

    return str;
}

char *num_to_str(int num) {
    const int num_len = (int)log10(num + 1) + 1 + 1;  // last +1 for the null terminator
    char *str = (char *)malloc(sizeof(char) * num_len + 1);
    snprintf(str, sizeof(char) * num_len, "%d", num);
    return str;
}

char *str_concat(const char *str1, const char *str2) {
    const int str1_len = strlen(str1);
    const int str2_len = strlen(str2);
    const int total_len = str1_len + str2_len + 1;

    char *result = (char *)malloc(sizeof(char) * total_len);
    strcpy(result, str1);
    strcat(result, str2);

    return result;
}

char *str_dup(char *str) {
    const int str_size = strlen(str);
    char *new_str = malloc(sizeof(char) * (str_size + 1));
    strncpy(new_str, str, str_size);
    return new_str;
}

unsigned concatenate(unsigned x, unsigned y) {
    unsigned pow = 10;
    while (y >= pow) pow *= 10;
    return x * pow + y;
}

long factorial(int n) {
    if (n == 0) {
        return 1;
    }

    return n * factorial(n - 1);
}

void beep(void) { printf("\a"); }

char pause_execution(const char *message) {
    printf("%s\n", message);
    return getchar();
}

int concat_bits(int a, int b) { return (a << get_used_bits(a)) | b; }

int get_bit(int n, int k) {
    int mask = 1 << k;
    int masked_n = n & mask;
    int thebit = masked_n >> k;
    return thebit;
}

int get_higher_bit_pos(unsigned int num) {
    int cnt = 0;

    while (num) {
        cnt++;
        num = num >> 1;
    }

    return cnt - 1;
}

int clear_bit(int n, int k) {
    int mask = ~(1 << k);
    return n & mask;
}

int clear_bits(int n, int from, int to) {
    for (int i = from; i <= to; i++) {
        n = clear_bit(n, i);
    }
    return n;
}

int set_bit(int n, int k) {
    int mask = 1 << k;
    return n | mask;
}
