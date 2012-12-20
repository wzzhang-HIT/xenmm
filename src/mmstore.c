#include <stdlib.h>
#include <xs.h>
#include <stdio.h>
#include <string.h>
#include "mmstore.h"

static struct xs_handle* g_h;//guest_handle
static struct xs_handle* h_h;//host_handle
struct WatchLock{
    char* token;
};

MMRetCode s_g_init()
{
    g_h = xs_domain_open();
    if(g_h == NULL){
        perror("s_g_init:");
        return MM_FAILED;
    }
    return MM_OK;
}

void s_g_close()
{
    xs_close(g_h);
}

void s_g_write_mem(MemInfo mem)
{
    char buf[512];
    xs_transaction_t t;
    t = xs_transaction_start(g_h);

    snprintf(buf,sizeof(buf),"%llu",mem.free_mem);
    xs_write(g_h, t, "memory/free", buf, strlen(buf));
    snprintf(buf, sizeof(buf), "%llu",mem.tot_mem);
    xs_write(g_h, t, "memory/tot", buf, strlen(buf));

    xs_transaction_end(g_h, t, 0);
}

MMRetCode s_h_init()
{
    h_h = xs_daemon_open();
    if(h_h == NULL){
        perror("s_h_init:");
        return MM_FAILED;
    }
    return MM_OK;
}

void s_h_close()
{
    xs_close(h_h);
}

void s_h_list_domains(Domain0* h)
{
    char** buf;
    uint buf_len;
    uint num;
    xs_transaction_t t = xs_transaction_start(h_h);
    buf = xs_directory(h_h, t, "/local/domain", &num);
    int i=0;
    for(i=0;i<num;i++){
        printf("%s\n",buf[i]);
    }
    free(buf);
    xs_transaction_end(h_h, t, 0);
}
