/*
 * File: /src/lib/clock.c
 * Project: mpp-cpu
 * File Created: Thursday, 1st January 1970 12:00:00 am
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Saturday, 16th April 2022 4:10:10 pm
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */

int last_clock_tick = 0;

int get_clock_tick(void) {
    last_clock_tick = !last_clock_tick;
    return last_clock_tick;
}