#ifndef MM_STORE_H_H_
#define MM_STORE_H_H_

#include "type.h"
#include <stdlib.h>
typedef struct WatchLock WatchLock;
typedef void (WatchCallback)(const char* buf,size_t len,void* userdata);

MMRetCode s_g_init();
void s_g_close();
void s_g_write_mem(MemInfo);
WatchLock* s_g_watch_mem(WatchCallback,void* userdata);

MMRetCode s_h_init();
void s_h_close();
void s_h_list_domains(Domain0*);
WatchLock* s_h_watch_mem(Domain*,WatchCallback,void* userdata);


void s_unwatch(WatchLock*);
#endif
