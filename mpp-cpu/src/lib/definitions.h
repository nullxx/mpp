/*
 * File: /src/lib/definitions.h
 * Project: mpp-cpu
 * File Created: Friday, 15th April 2022 11:32:56 am
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Friday, 15th April 2022 11:33:22 am
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */

#ifndef definitions_h
#define definitions_h
#define WORD_SIZE_BIT 32

typedef struct {
    int bits[WORD_SIZE_BIT];
} Word;

int word_to_int(Word bw);
Word int_to_word(int value);
void initialize_word(Word *bw, int value);
int get_used_bits(Word w);
void print_word(Word w);

#endif