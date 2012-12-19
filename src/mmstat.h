#ifndef MM_STAT_H_H_
#define MM_STAT_H_H_
#include "type.h"

typedef void StatInfo;

void stat_global_init();
void stat_global_free();

#define STAT_BASIC 0

StatInfo* stat_now(int stat_what);
void stat_free(StatInfo*);

MemInfo stat_read_mem(StatInfo*);
#endif
