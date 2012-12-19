#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "type.h"
#include "mmstore.h"

int main()
{
    int i=0;
    MemInfo mem;
    if(s_g_init())
        return;

    ulong pagesize = sysconf(_SC_PAGESIZE);
    while(i<1000){
        ulong tot = sysconf(_SC_PHYS_PAGES);
        ulong free = sysconf(_SC_AVPHYS_PAGES);
        mem.tot_mem = tot;
        mem.free_mem = free;
        printf("tot:%lu,free:%lu\n",tot,free);
        s_g_write_mem(mem);
        sleep(1);
    }

    s_g_close();

}
