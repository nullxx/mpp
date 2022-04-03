//
//  mxdir.h
//  mpp-cpu
//
//  Created by https://github.com/nullxx on 23/3/22.
//

#ifndef mxdir_h
#define mxdir_h
#include <stdbool.h>

void init_mxdir(void);
void shutdown_mxdir(void);
void run_mxdir(void);  // <==

// control loadbits
bool set_seldir_lb(unsigned int bin);
// -- control loadbits
#endif /* mxdir_h */
