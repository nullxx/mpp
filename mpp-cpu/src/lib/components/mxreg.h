/*
 * File: /src/lib/components/mxreg.h
 * Project: mpp-cpu
 * File Created: Wednesday, 30th March 2022 11:42:16 am
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Wednesday, 30th March 2022 11:42:18 am
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */

#ifndef mxreg_h
#define mxreg_h
#include <stdbool.h>

void init_mxreg(void);
void shutdown_mxreg(void);
void run_mxreg(void);

bool set_selreg_lb(unsigned int sel);

#endif