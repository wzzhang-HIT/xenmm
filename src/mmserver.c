#include "type.h"
#include "mmstore.h"

void domainu_mem_change(const char* buf,size_t len,void* data)
{
}

int main()
{
    if(s_h_init()==MM_FAILED){
        return -1;
    }
    s_h_list_domains();
    Domain* domainu;
    LIST_FOREACH(domainu,&domain0.domainu,entries){
        s_h_watch_guest_mem(domainu, domainu_mem_change, domainu);
    }

    while(1){
    s_h_wait_change();
    }

    s_h_close();
}
