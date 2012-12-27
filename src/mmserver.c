/*=============================================================================
#     FileName: mmserver.c
#         Desc: the main file of memory manage server 
#       Author: xiehuc
#        Email: xiehuc@gmail.com
#     HomePage: 
#      Version: 0.0.1
#   LastChange: 2012-12-25 17:10:04
#      History:
=============================================================================*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "type.h"
#include "mmstore.h"
#include "mmctrl.h"
#include "solve.h"

#define tax_rate 0.75
#define tau tax_rate
#if 0
static void domainu_mem_change(void* data)
{
    Domain* d = data;

    s_h_read_domain_mem(d);
}
#endif

static void build_linear_equ()
{
    memset(_a_,sizeof(_a_),0);
    memset(_b_,sizeof(_b_),0);
    memset(_x_,sizeof(_x_),0);
    int len = 0;
    double _N_ = 0;
    double _A0_ = 0;
    Domain* d;
    double _Ai_;
    int i;

    LIST_FOREACH(d,&domain0.domainu,entries){
        _N_ += (double)d->tot_mem;
        _Ai_ = (double)(d->tot_mem - d->free_mem);
        if(len==0){
            _A0_ = _Ai_;
        }else{
            _b_[len] = tau*(_Ai_-_A0_);
            _a_[len][0] = 1;
            _a_[len][len] = -1;
        }
        len++;
    }
    _b_[0] = _N_;
    for(i=0;i<len;i++){
        _a_[0][i] = 1;
    }
    solve_line_equations(_a_, _b_, len, _x_);
    i=0;
    printf("[");
    LIST_FOREACH(d,&domain0.domainu,entries){
        printf(" %llu|%llu ",d->tot_mem,d->free_mem);
    }
    printf("]\n");
    printf("[");
    for(i=0;i<len;i++){
        printf(" %llu ",(mem_t)_x_[i]);
    }
    printf("]\n\n");
    i = 0;
    LIST_FOREACH(d,&domain0.domainu,entries){
        d->target_mem = _x_[i++];
    //    d->tot_mem = _x_[i++];
        s_h_set_domain_mem(d);
        ctrl_update_domain_mem(d);
    }
}

int main()
{
    if(s_h_init()==MM_FAILED){
        return -1;
    }
    ctrl_init();
    s_h_list_domains();
    if(LIST_EMPTY(&domain0.domainu)){
        fprintf(stderr,"it seems you didn't run mmclient in any guest vm.\n"
                "you can install mmclient in your guest vms.\n"
                "and use `sudo services mmclientd start`\n"
                "or `sudo /etc/init.d/mmclientd start` to run mmclient.\n"
               );
        s_h_close();
        return 0;
    }
    /*LIST_FOREACH(domainu,&domain0.domainu,entries){
        s_h_watch_guest_mem(domainu, domainu_mem_change, domainu);
    }*/

    //s_h_wait_change();
    Domain* d;
    while(1){
        sleep(5);
        //s_h_wait_change();
        LIST_FOREACH(d,&domain0.domainu,entries){
            s_h_read_domain_mem(d);
        }
        build_linear_equ();
    }

    s_h_close();
    ctrl_close();
}
