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
#include <xenctrl.h>

static xc_interface* c_h;
static xentoollog_logger xc_logger;



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
    if(abs(allocated-d->tg_mem)<1024*50) return;
    uint32_t target = allocated;
    xc_domain_set_pod_target(c_h, d->id, target / 4, 0, 0, 0);
}
