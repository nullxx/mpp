/*
 * File: /src/linker/linker.h
 * Project: mpp-cpu
 * File Created: Friday, 22nd April 2022 11:25:53 pm
 * Author: https://github.com/nullxx (mail@nullx.me)
 * -----
 * Last Modified: Friday, 22nd April 2022 11:25:55 pm
 * Modified By: https://github.com/nullxx (mail@nullx.me)
 */

#ifndef linker_h
#define linker_h
typedef void (*update_ui_fn)(void);

update_ui_fn get_update_ui_fn(void);
#endif