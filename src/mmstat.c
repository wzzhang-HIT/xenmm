#include <xenstat.h>
#include <stdlib.h>
#include "mmstat.h"


static struct {
    xenstat_handle* xs_h;
}global = {0};

void stat_global_init()
{
    global.xs_h = xenstat_init();

}

void stat_global_free()
{
    xenstat_uninit(global.xs_h);
    global.xs_h = NULL;
}

StatInfo* stat_now(int stat_what)
{
    return xenstat_get_node(global.xs_h, stat_what);
}

void stat_free(StatInfo* info)
{
    xenstat_free_node(info);
}
MemOf stat_mem_all(StatInfo* info){
    MemOf mf;
    mf.tot_mem = xenstat_node_tot_mem(info);
    mf.free_mem = xenstat_node_free_mem(info);
}
MemInfo stat_fetch_mem(StatInfo* info)
{
    MemInfo mem;
    static int i=0,num=0;
    if(num == 0){
        num = xenstat_node_num_domains(info);
        i = 0;
    }
    if( i = num ){
        i = num = 0;
        mem.id = -1;
        return mem;
    }
    xenstat_domain* domain = xenstat_node_domain_by_index(info, i++);
    mem.id = xenstat_domain_id(domain);
    mem.d_max_mem = xenstat_domain_max_mem(domain);
    mem.d_cur_mem = xenstat_domain_cur_mem(domain);
    return mem;
}
