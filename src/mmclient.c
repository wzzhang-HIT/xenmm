#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include "type.h"
#include "mmstore.h"

int main()
{
    MemInfo mem;
    if(s_g_init())
        return 0;

    /*
    ulong pagesize = sysconf(_SC_PAGESIZE)/1024;//kb
    while(1){
        ulong tot = sysconf(_SC_PHYS_PAGES)*pagesize;
        ulong free = sysconf(_SC_AVPHYS_PAGES)*pagesize;
        mem.tot_mem = tot;
        mem.free_mem = free;
        printf("tot:%lu,free:%lu\n",tot,free);
        s_g_write_mem(mem);
        sleep(1);
    }*/

    char buf[4096];
    char* ptr;
    while(1){
        int fd = open("/proc/meminfo",O_RDONLY);
        size_t len = read(fd,buf,sizeof(buf)-1);
        buf[len]='\0';
        ptr = strstr(buf,"MemTotal:")+strlen("MemTotal:");
        mem.tot_mem = strtoull(ptr,NULL,10);
        ptr = strstr(ptr,"MemFree:")+strlen("MemFree:");
        unsigned long long free = strtoull(ptr,NULL,10);
        //ptr = strstr(ptr,"Buffers:")+strlen("Buffers:");
        //unsigned long long buffer = strtoull(ptr,NULL,10);
        //ptr = strstr(ptr,"Cached:")+strlen("Cached:");
        //unsigned long long cached = strtoull(ptr,NULL,10);
        //mem.free_mem = free+cached;
        mem.free_mem = free;
        printf("tot:%llu,free:%llu\n",mem.tot_mem,mem.free_mem);
        s_g_write_mem(mem);
        close(fd);
        sleep(1);
    }

    s_g_close();

}
