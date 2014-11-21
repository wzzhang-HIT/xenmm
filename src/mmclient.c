#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include "type.h"
#include "mmstore.h"

static void read_mem(MemInfo* mem)
{
   char buf[4096];
   char* ptr;

   int fd = open("/proc/meminfo",O_RDONLY);
   size_t len = read(fd,buf,sizeof(buf)-1);
   buf[len]='\0';
   ptr = strstr(buf,"MemTotal:")+strlen("MemTotal:");
   mem->tot_mem = strtoull(ptr,NULL,10);
   ptr = strstr(ptr,"MemFree:")+strlen("MemFree:");
   unsigned long long free = strtoull(ptr,NULL,10);
   //ptr = strstr(ptr,"Buffers:")+strlen("Buffers:");
   //unsigned long long buffer = strtoull(ptr,NULL,10);
   ptr = strstr(ptr,"Cached:")+strlen("Cached:");
   unsigned long long cached = strtoull(ptr,NULL,10);
   //计算方法:把cached看作free,纠正内存计算
   mem->free_mem = free+cached;
   //mem.free_mem = free;
   close(fd);
}

#ifndef IMPLEMENT_CLIENT
// with this macro, it means there are another implement for client. so, we
// shouldn't compile main function 
int main()
{
    MemInfo mem;
    if(s_g_init())
        return 0;

#if 0
    /*
     * 使用sysconf获取内存信息的方法，因为数据少而放弃
     */
    ulong pagesize = sysconf(_SC_PAGESIZE)/1024;//kb
    while(1){
        ulong tot = sysconf(_SC_PHYS_PAGES)*pagesize;
        ulong free = sysconf(_SC_AVPHYS_PAGES)*pagesize;
        mem.tot_mem = tot;
        mem.free_mem = free;
        printf("tot:%lu,free:%lu\n",tot,free);
        s_g_write_mem(mem);
        sleep(1);
    }
#endif

    while(1){
        read_mem(&mem);
        printf("tot:%lld,free:%lld\n",mem.tot_mem,mem.free_mem);
        s_g_write_mem(mem);
        sleep(1);
    }

    s_g_close();

}
#endif
