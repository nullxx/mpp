/*
 * File: /src/lib/components/cu.h
 * Project: mpp-cpu
 * File Created: Saturday, 2nd April 2022 1:23:02 pm
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Saturday, 2nd April 2022 1:23:04 pm
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */

#ifndef cu_h
#define cu_h
void init_cu(void);
void shutdown_cu(void);
void run_cu(int);
double run_clock_cycle(void);
#endif