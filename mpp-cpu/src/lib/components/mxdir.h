//
//  mxdir.h
//  mpp-cpu
//
//  Created by https://github.com/nullxx on 23/3/22.
//

#ifndef mxdir_h
#define mxdir_h

void init_mxdir(void);
void shutdown_mxdir(void);
void run_mxdir(void);  // <==

// control loadbits
void set_seldir_lb(void);
void reset_seldir_lb(void);
// -- control loadbits
#endif /* mxdir_h */
