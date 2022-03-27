/*
 * File: /src/lib/components/sp.h
 * Project: mpp-cpu
 * File Created: Thursday, 25th March 2022 15:42:00 pm
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Thursday, 25th March 2022 7:12:02 pm
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */

#ifndef pc_h

void init_pc(void);
void shutdown_pc(void);
void run_pc(void);

void set_pchcar_lb(void);
void reset_pchcar_lb(void);
void set_pclcar_lb(void);
void reset_pclcar_lb(void);
void set_pccar_lb(void);
void reset_pccar_lb(void);

#endif /* pc_h */