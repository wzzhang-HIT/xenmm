#include <stdlib.h>
#include <xs.h>
#include <stdio.h>
#include <string.h>
#include "mmstore.h"

static struct xs_handle* g_h;//guest_handle
static struct xs_handle* h_h;//host_handle
struct WatchLock{
    char token[12];
    WatchCallback func;
    void* data;
    LIST_ENTRY(WatchLock) entries;
};

static LIST_HEAD(,WatchLock) watch_head;


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

    snprintf(buf,sizeof(buf),"%lld",mem.free_mem);
    xs_write(g_h, t, "memory/free", buf, strlen(buf));
    snprintf(buf, sizeof(buf), "%lld",mem.tot_mem);
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
    uint i=0;
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
    if(!g||!func) return NULL;
    static int token = 0;
    WatchLock* lock = malloc0(sizeof(*lock));
    lock->func = func;
    lock->data = data;

    LIST_INSERT_HEAD(&watch_head,lock,entries);

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
    uint i;
    char *path,*token;
    WatchLock* lock;
    for(i=0;i<num;i+=2){
        path = list[i];
        token = list[i+1];

        LIST_FOREACH(lock,&watch_head,entries){
            if(!strcmp(lock->token,token)){
                lock->func(lock->data);
            }
        }

    }
    UNUSED(path);
    free(list);
}

void s_h_read_domain_mem(Domain* d)
{
    if(!d) return ;
    char path[512];
    char* buf;
    uint buf_len;

    xs_transaction_t t = xs_transaction_start(h_h);
    snprintf(path,sizeof(path),"/local/domain/%u/memory/free",d->id);
    buf = xs_read(h_h, t, path, &buf_len);
    d->free_mem = strtoll(buf,NULL,10);
    free(buf);

    snprintf(path, sizeof(path), "/local/domain/%u/memory/tot",d->id);
    buf = xs_read(h_h, t, path, &buf_len);
    d->tot_mem = strtoll(buf,NULL,10);
    free(buf);

    snprintf(path, sizeof(path), "/local/domain/%u/memory/target",d->id);
    buf = xs_read(h_h, t, path, &buf_len);
    d->tg_mem = strtoll(buf, NULL, 10);
    free(buf);

    xs_transaction_end(h_h, t, false);
}

char* s_h_read_name(Domain* d)
{
   if(!d) return NULL;
   char path[512];
   char* buf;
   uint buf_len;

   xs_transaction_t t = xs_transaction_start(h_h);
   snprintf(path,sizeof(path),"/local/domain/%u/name",d->id);
   buf = xs_read(h_h, t, path, &buf_len);
   xs_transaction_end(h_h, t, false);
   return buf;
}

void s_h_set_domain_mem(Domain* d,mem_t allocate)
{
    if(!d) return ;
    if(abs(allocate - d->tg_mem)<ACCURACY) return;
	 if(d->tg_mem < d->min_mem) return;

    char path[512];
    char target[64];
    xs_transaction_t t = xs_transaction_start(h_h);
    snprintf(path, sizeof(path), "/local/domain/%u/memory/target",d->id);
    snprintf(target,sizeof(target),"%lld",allocate);
    xs_write(h_h, t, path, target, strlen(target));
    xs_transaction_end(h_h, t, 0);
}
