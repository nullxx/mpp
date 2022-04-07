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

int hex_to_int(const char *hex) {
    int number = (int)strtol(hex, NULL, 16);
    return number;
}

char *int_to_hex(int num) {
    int size = 4;
    char *result = (char *)malloc(sizeof(char) * (size + 1));
    if (result == NULL) {
        goto error;
    }

    snprintf(result, size, "%x", num);

    while (1) {
        const int next_size = size + 1;
        char *result2 = (char *)malloc(sizeof(char) * (next_size));
        snprintf(result2, next_size, "%x", num);

        if (!strcmp(result, result2)) {
            free(result2);
            break;
        };

        free(result);
        result = result2;

        size++;
    }
error:
    return result;
}

int random_int(const int min, const int max) {
    int r = min + rand() % (max - min);
    return r;
}

unsigned long long str_to_bin(const char *s) {
    unsigned long long i = 0;
    while (*s) {
        i <<= 1;
        i += *s++ - '0';
    }
    return i;
}

int get_bin_len(unsigned long long bin) {
    const int bin_len = (int)log10(bin + 1) + 1;
    return bin_len;
}

char *bin_to_str(unsigned long long bin) {
    const int bin_len = get_bin_len(bin);

    const size_t size = sizeof(char) * (bin_len + 1);
    char *str = (char *)malloc(size);

    snprintf(str, size, "%llu", bin);
    return str;
}

int bin_to_int(unsigned long long bin) {
    int dec = 0, i = 0, rem;

    while (bin != 0) {
        rem = bin % 10;
        bin /= 10;
        dec += rem * pow(2, i);
        ++i;
    }

    return dec;
}

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
unsigned long long int_to_bin(unsigned k) {
    if (k == 0) return 0;
    if (k == 1) return 1;                       /* optional */
    return (k % 2) + 10 * int_to_bin(k / 2);
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

        if (_first) {
            str = (char *)malloc(next_size);
            if (str == NULL) return NULL;

            initialize_str(str, 0, next_size, 0);
        } else {
            str = (char *)realloc(str, next_size);
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
    char *new_str = malloc(sizeof(char) * (str_size+1));
    strncpy(new_str, str, str_size);
    return new_str;
}