/*=============================================================================
#     FileName: mmctrl.c
#         Desc: the ctrl interface of memory management
#       Author: xiehuc
#        Email: xiehuc@gmail.com
#     HomePage: 
#      Version: 0.0.1
#   LastChange: 2012-12-25 17:10:10
#      History:
=============================================================================*/
#include "mmctrl.h"
#include "mmstore.h"
#include "config.h"


#ifdef WITH_LIBXL
#include <libxl.h>
static xentoollog_logger xc_logger;
static libxl_ctx* ctx;

void ctrl_init()
{
   libxl_ctx_alloc(&ctx, LIBXL_VERSION, 0, &xc_logger);
}

void ctrl_close()
{
   libxl_ctx_free(ctx);
}
void ctrl_update_domain_mem(Domain* d,mem_t allocated)
{
    if(!d) return;
    //大于精度才进行调整
    if(abs(allocated-d->tg_mem)<ACCURACY) return;
    if(allocated < 0) return;
    int32_t target = allocated;
    libxl_set_memory_target(ctx, d->id, target, 0, 1);
}
// no need to implement
void ctrl_read_domains_maxmem()
{
}

#else

#include <xenctrl.h>
static xentoollog_logger xc_logger;
static xc_interface* c_h;
void ctrl_init()
{
    c_h = xc_interface_open(&xc_logger, &xc_logger, 0);
}

void ctrl_close()
{
    xc_interface_close(c_h);
}

void ctrl_update_domain_mem(Domain* d,mem_t allocated)
{
    if(!d) return;
    //大于精度才进行调整
    s_h_set_domain_mem(d, allocated);

    if(abs(allocated-d->tg_mem)<ACCURACY) return;
    uint32_t target = allocated;
    //参考libxl源代码相关功能函数
    if(target > d->max_mem){
        xc_domain_setmaxmem(c_h, d->id, target+10240);
        d->max_mem = target+10240;
    }
    xc_domain_set_pod_target(c_h, d->id, target / 4, 0, 0, 0);
}
void ctrl_read_domains_maxmem()
{
    xc_domaininfo_t domain_infos[256];
    int domains_num = xc_domain_getinfolist(c_h, 0, 256, domain_infos);
    int i;
    for(i=0;i<domains_num;i++){
        int id = domain_infos[i].domain;
        Domain* domain = get_domain(id);
        if(domain == NULL) continue;
        domain->max_mem = domain_infos[i].max_pages * domain0.page_size;
    }
}
#endif

