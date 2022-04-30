/*
 * File: /src/lib/definitions.c
 * Project: mpp-cpu
 * File Created: Friday, 15th April 2022 11:32:59 am
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Friday, 15th April 2022 11:33:36 am
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */

#include "definitions.h"

#include <stdio.h>

#include "utils.h"

int word_to_int(Word w) {
    int value = w.bits[WORD_SIZE_BIT - 1];
    for (int i = WORD_SIZE_BIT - 2; i >= 0; i--) {
        value = (value << 1) | w.bits[i];
    }

    return value;
}

Word int_to_word(int value) {
    Word w;
    initialize_word(&w, 0);
    for (int i = 0; i < WORD_SIZE_BIT; i++) {
        w.bits[i] = get_bit(value, i);
    }

    return w;
}

int get_used_bits(Word w) {
    for (int i = WORD_SIZE_BIT - 1; i >= 0; i--) {
        if (w.bits[i] == 1) {
            return i + 1;
        }
    }

    return 0;
}

void print_word(Word w) {
    for (int i = WORD_SIZE_BIT - 1; i >= 0; i--) {
        printf("%d", w.bits[i]);
    }
    printf("\n");
}

void initialize_word(Word *bw, int value) {
    for (int i = 0; i < WORD_SIZE_BIT; i++) {
        bw->bits[i] = value;
    }
}