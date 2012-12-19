#ifndef MM_STORE_H_H_
#define MM_STORE_H_H_

#include "type.h"

MMRetCode s_g_init();
void s_g_close();
void s_g_write_mem(MemInfo);

#endif
