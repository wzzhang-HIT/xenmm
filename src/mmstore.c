#include <stdlib.h>
#include <xs.h>
#include <stdio.h>
#include <string.h>
#include "mmstore.h"

static struct xs_handle* g_h;//guest_handle
static struct xs_handle* h_h;//host_handle
struct WatchLock{
    char token[12];
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

void s_h_list_domains()
{
    char** ls;
    char path[512];
    char* result;
    Domain0* h = &domain0;

    uint num,len;
    xs_transaction_t t = xs_transaction_start(h_h);
    ls = xs_directory(h_h, t, "/local/domain", &num);
    int i=0;
    for(i=0;i<num;i++){
        snprintf(path,sizeof(path),"/local/domain/%s/memory/free",ls[i]);
        result = xs_read(h_h, t, path, &len);
        if(result !=NULL){
            free(result);
            Domain* d = malloc(sizeof(*d));
            d->id = atoi(ls[i]);
            LIST_INSERT_HEAD(&h->domainu,d,entries);
        }
    }
    free(ls);
    xs_transaction_end(h_h, t, 0);
}

WatchLock* s_h_watch_guest_mem(Domain* g,WatchCallback func,void* data)
{
    if(!g||!func) return;
    static int token = 0;
    WatchLock* lock = malloc0(sizeof(*lock));
    snprintf(lock->token,sizeof(lock->token),"%d",token++);
    char path[128];
    snprintf(path,sizeof(path),"/local/domain/%u/memory/free",g->id);
    if(!xs_watch(h_h, path, lock->token))
        perror(__PRETTY_FUNCTION__);
    return lock;
}

void s_h_wait_change()
{
    char** list;
    uint num;
    list = xs_read_watch(h_h, &num);
    int i;
    for(i=0;i<num;i++){
        printf("%s\n",list[i]);
    }
    free(list);
}
