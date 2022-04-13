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

int get_num_len(unsigned long long bin) {
    const int len = (int)log10(bin + 1) + 1;
    return len;
}

char *dec_to_hex(int num) {
    int size = get_num_len(num);
    char *result = (char *)malloc(sizeof(char) * (size + 1));
    if (result == NULL) {
        return NULL;
    }

    snprintf(result, size, "%x", num);
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
    const int bin_len = get_num_len(bin);

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

char *itoa(int num) {
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