//
//  multiplexer.h
//  mpp-cpu
//
//  Created by https://github.com/nullxx on 23/3/22.
//

#ifndef multiplexer_h
#define multiplexer_h

typedef unsigned long long MXInput;

MXInput run_4x1_mx(unsigned int sel, MXInput i1, MXInput i2, MXInput i3, MXInput i4);
MXInput run_2x1_mx(unsigned int sel, MXInput i1, MXInput i2);
#endif /* multiplexer_h */
