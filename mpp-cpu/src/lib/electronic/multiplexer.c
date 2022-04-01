//
//  multiplexer.c
//  mpp-cpu
//
//  Created by https://github.com/nullxx on 23/3/22.
//

#include "multiplexer.h"

#include <stdbool.h>

#include "../utils.h"

/**
 * Returns -1 when invalid
 */
MXInput run_4x1_mx(unsigned int sel, MXInput i1, MXInput i2, MXInput i3, MXInput i4) {
    const bool is_input_len_valid = get_bin_len(i1) == get_bin_len(i2) == get_bin_len(i3) == get_bin_len(i4);
    if (!is_input_len_valid) return -1;

    switch (sel) {
        case 00:
            return i1;

        case 01:
            return i2;

        case 10:
            return i3;

        case 11:
            return i4;

        default:
            return -1;
    }
}

/**
 * Returns -1 when invalid
 */
MXInput run_2x1_mx(unsigned int sel, MXInput i1, MXInput i2) {
    const bool is_input_len_valid = get_bin_len(i1) == get_bin_len(i2);
    if (!is_input_len_valid) return -1;

    switch (sel) {
        case 0:
            return i1;

        case 1:
            return i2;

        default:
            return -1;
    }
}