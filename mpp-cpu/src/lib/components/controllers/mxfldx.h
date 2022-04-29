/*
 * File: /src/lib/components/controllers/mxfldx.h
 * Project: mpp-cpu
 * File Created: Friday, 1st April 2022 11:52:21 pm
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Friday, 1st April 2022 11:52:23 pm
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */

#ifndef mxfldx_h
#define mxfldx_h
#include "../../definitions.h"

void cll_init_mxfldx(void);
void cll_shutdown_mxfldx(void);
void cll_run_mxfldx(void);

int cll_get_selfl_lb_value(void);
Word cll_get_last_bus_data(void);
#endif