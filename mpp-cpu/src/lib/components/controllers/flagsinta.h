/*
 * File: /src/lib/components/controllers/flagsinta.h
 * Project: mpp-cpu
 * File Created: Saturday, 2nd April 2022 12:34:46 pm
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Saturday, 2nd April 2022 12:34:56 pm
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */
#ifndef flagsinta_h
#define flagsinta_h
void cll_init_flagsinta(void);
void cll_shutdown_flagsinta(void);
void cll_run_flagsinta(void);

void cll_set_flbus_lb(void);
void cll_reset_flbus_lb(void);
#endif