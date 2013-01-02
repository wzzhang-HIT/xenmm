#include "type.h"
#include <string.h>
#include <unistd.h>

Domain0 domain0;

void* malloc0(size_t sz)
{
    void* ret = malloc(sz);
    memset(ret,sz,0);
    return ret;
}

void domain0_init()
{
    domain0.page_size = sysconf(_SC_PAGE_SIZE);
}

Domain* get_domain(uint id)
{
    Domain* ret;
    LIST_FOREACH(ret,&domain0.domainu,entries){
        if(ret->id == id) return ret;
    }
    return NULL;
}
