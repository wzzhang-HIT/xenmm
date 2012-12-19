#include <stdio.h>
#include <stdlib.h>
#include "type.h"
#include "mmstore.h"

int main()
{
    int i=0;
    MemInfo mem;
    s_g_init();

    while(i<1000){
        mem.free_mem = i++;
        s_g_write_mem(mem);
        sleep(1);
    }

    s_g_close();

}
