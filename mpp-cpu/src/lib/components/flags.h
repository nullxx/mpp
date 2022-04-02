/*
 * File: /src/lib/components/flags.h
 * Project: mpp-cpu
 * File Created: Saturday, 2nd April 2022 11:00:15 am
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Saturday, 2nd April 2022 11:00:17 am
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */
#ifndef flags_h
#define flags_h
void init_flags(void);
void shutdown_flags(void);
void run_flags(void);

void set_flcar_lb(void);
void reset_flcar_lb(void);
#endif