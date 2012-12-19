#include <stdlib.h>
#include <xs.h>
#include <stdio.h>
#include <string.h>
#include "mmstore.h"

static struct xs_handle* g_h;

void s_g_init()
{
    g_h = xs_domain_open();
}

void s_g_close()
{
    xs_close(g_h);
}

void s_g_write_mem(MemInfo mem)
{
    char buf[512];
    xs_transaction_t t;
    xs_transaction_start(g_h);

    snprintf(buf,sizeof(buf),"%ld",mem.free_mem);
    xs_write(g_h, t, "memory/free", buf, strlen(buf));

    xs_transaction_end(g_h, t, 0);
}
