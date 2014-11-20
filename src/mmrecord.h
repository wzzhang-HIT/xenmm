#ifndef MM_RECORD_H_H
#define MM_RECORD_H_H

#include "type.h"
/* start record domain's memory info */
void record_begin(Domain* d, const char* dir);
/* write domain's memory info to record file */
void record_mem(Domain*);
/* finish record */
void record_end(Domain*);

#endif
