#include "type.h"
#include <string.h>

Domain0 domain0;

void* malloc0(size_t sz)
{
    void* ret = malloc(sz);
    memset(ret,sz,0);
    return ret;
}
