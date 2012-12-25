/*=============================================================================
#     FileName: mmserver.c
#         Desc: the main file of memory manage server 
#       Author: xiehuc
#        Email: xiehuc@gmail.com
#     HomePage: 
#      Version: 0.0.1
#   LastChange: 2012-12-22 19:35:07
#      History:
=============================================================================*/
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
    if(LIST_EMPTY(&domain0.domainu)){
        fprintf(stderr,"it seems you didn't run mmclient in any guest vm.\n"
                "you can install mmclient in your guest vms.\n"
                "and use `sudo services mmclientd start`\n"
                "or `sudo /etc/init.d/mmclientd start` to run mmclient.\n"
               );
        s_h_close();
        return;
    }
    LIST_FOREACH(domainu,&domain0.domainu,entries){
        s_h_watch_guest_mem(domainu, domainu_mem_change, domainu);
    }

    while(1){
    s_h_wait_change();
    }

    s_h_close();
}
