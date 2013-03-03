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
#include <signal.h>
#include <math.h>
#include "type.h"
#include "mmstore.h"
#include "mmctrl.h"
#include "solve.h"
#include "mmrecord.h"

#define tau tax_rate
static int prog_quit = 0;
static double old_tau = 0.0;
#if 0
static void domainu_mem_change(void* data)
{
    Domain* d = data;

    s_h_read_domain_mem(d);
}
void matrix_print(int len)
{
    int i,j;
    for(i=0;i<len;i++){
        printf("[");
        for(j=0;j<len;j++){
            printf("%.0lf,",_a_[i][j]);
        }
        printf("]\n");
    }
}
void vector_print(int len)
{
    int i;
    for(i=0;i<len;i++){
        printf("[%.0lf]\n",_b_[i]);
    }
}
#endif
static void build_linear_equ()
{
    int i;
    memset(_x_,sizeof(_x_),0);
#if SOLVE_EQU
    memset(_a_,sizeof(_a_),0);
    memset(_b_,sizeof(_b_),0);
    int len = 0;
    double _N_ = 0;
    double _A0_ = 0;
    Domain* d;
    double _Ai_;
    mem_t total;
    LIST_FOREACH(d,&domain0.domainu,entries){
        total = (abs(d->tg_mem - d->tot_mem)> 70*1024)?d->tot_mem:d->tg_mem;
        _N_ += (double)d->tg_mem;
        _Ai_ = (double)(total - d->free_mem);
        if(len==0){
            _A0_ = _Ai_;
        }else{
            _b_[len] = tau*(_A0_-_Ai_);
            _a_[len][0] = 1;
            _a_[len][len] = -1;
        }
        len++;
    }
    //_b_[0] = _N_;
    _b_[0] = 1024*1024*len;
    for(i=0;i<len;i++){
        _a_[0][i] = 1;
    }
#if 0
    matrix_print(len);
    vector_print(len);
#endif
    solve_line_equations(_a_, _b_, len, _x_);
#else
    double Amax = 0;
    double Amean = 0;
    double Total;
    double Tau;
    int len = 0;
    Domain* d;
    LIST_FOREACH(d,&domain0.domainu,entries){
        mem_t total = (abs(d->tg_mem - d->tot_mem)> 70*1024)?d->tot_mem:d->tg_mem;
        double A=(double)(total-d->free_mem);
        _x_[len]=A;
        if(A>Amax) Amax=A;
        Amean+=A;
        len++;
    }
    Total = 1024*1024*len;
    Amax/=Total;
    Amean/=len;
#if AUTO_TAX_RATE
    //Tau = sqrt(1-pow(1-Amax,2));
    Tau = (Amax-1.0/len)/(Amax-Amean/Total);
    Tau = Tau * 0.8 + 0.2;
    Tau = (Tau<0)?0:((Tau>1)?1:Tau);
    Tau = Tau>old_tau?Tau:old_tau-(old_tau-Tau)/5;
    printf("Tau:%lf\n",Tau);
    old_tau = Tau;
#else
    Tau = tau;
#endif
    double Part = Total/len-Tau*Amean;
    i=0;
    LIST_FOREACH(d,&domain0.domainu,entries){
        _x_[i]=Part+Tau*_x_[i];
        i++;
    }
#endif
    i=0;
    LIST_FOREACH(d,&domain0.domainu,entries){
        printf("%d:[ tg:%llu use:%llu free:%llu ]\n",d->id,d->tg_mem,d->tot_mem-d->free_mem,d->free_mem);
    }
    printf("Target:[");
    for(i=0;i<len;i++){
        printf(" %llu ",(mem_t)_x_[i]);
    }
    printf("]\n\n");
    i = 0;
    mem_t allocated;
    LIST_FOREACH(d,&domain0.domainu,entries){
        allocated = _x_[i++];
        s_h_set_domain_mem(d,allocated);
        ctrl_update_domain_mem(d,allocated);
    }
}
static void interupt_server(int sig)
{
    prog_quit = 1;
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
    signal(SIGINT, interupt_server);
    //=============print param=============
#if AUTO_TAX_RATE
    printf("[PARAM: TAU=AUTO]\n\n");
#else
    printf("[PARAM: TAU=%f]\n\n",tau);
#endif
    //=====================================
    /*LIST_FOREACH(domainu,&domain0.domainu,entries){
        s_h_watch_guest_mem(domainu, domainu_mem_change, domainu);
    }*/

    //s_h_wait_change();
    ctrl_read_domains_maxmem();
    Domain* d;
    LIST_FOREACH(d,&domain0.domainu,entries){
        record_begin(d);
    }
    while(!prog_quit){
        sleep(2);
        //s_h_wait_change();
        LIST_FOREACH(d,&domain0.domainu,entries){
            s_h_read_domain_mem(d);
            record_mem(d);
        }
        build_linear_equ();
    }

    LIST_FOREACH(d,&domain0.domainu,entries){
        record_end(d);
    }
    s_h_close();
    ctrl_close();
    return 0;
}
