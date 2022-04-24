/*
 * File: /src/lib/components/acumm.h
 * Project: mpp-cpu
 * File Created: Tuesday, 29th March 2022 10:19:49 pm
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Tuesday, 29th March 2022 10:19:52 pm
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */
#ifndef acumm_h
#define acumm_h

void init_acumm(void);
void shutdown_acumm(void);
void run_acumm(void);

void set_acbus_lb(void);
void reset_acbus_lb(void);
void set_accar_lb(void);
void reset_accar_lb(void);
#endif