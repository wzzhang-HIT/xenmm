#ifndef MM_STORE_H_H_
#define MM_STORE_H_H_

#include "type.h"
#include <stdlib.h>
typedef struct WatchLock WatchLock;
typedef void (*WatchCallback)(void* userdata);

/* store guest init */
MMRetCode s_g_init();
/* store guest close */
void s_g_close();
/* store guest write total, free memory to xenstore*/
void s_g_write_mem(MemInfo);
/* store guest watch memory change, if changed call callback with userdata */
WatchLock* s_g_watch_mem(WatchCallback,void* userdata);

/* store host init */
MMRetCode s_h_init();
/* store host close */
void s_h_close();
/* store host list all domains, fill domain0's field
 * fetch data by get_domain() */
void s_h_list_domains();
/* store host watch guest memory change */
WatchLock* s_h_watch_guest_mem(Domain*,WatchCallback,void* userdata);
/* not used */
void s_h_wait_change();
/* read domain's tot,free,target memory from xenstore */
void s_h_read_domain_mem(Domain*);
char* s_h_read_name(Domain* d);
/* set domain's target memory on xenstore */
void s_h_set_domain_mem(Domain* d,mem_t allocate);
#endif
