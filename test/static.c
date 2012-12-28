/*=============================================================================
#     FileName: static.c
#         Desc: static hold memory allocation test 
#       Author: xiehuc
#        Email: xiehuc@gmail.com
#     HomePage: 
#      Version: 0.0.1
#   LastChange: 2012-12-28 11:17:49
#      History:
=============================================================================*/

#include "util.h"
#include <stdio.h>
//adjust this value to set max mb size of memory
//malloced
void show_help()
{
    const char* help=
        "useage: mono low high\n"
        "example: mono 100M 500M\n";
    printf(help);
}
int main(int argc,char** argv)
{
    printf("type a mem value to allocate and keep,\n"
            "type q to quit\n");
    char str[256];
    ul tg_mem;
    while(1){
        printf(":>");
        scanf("%s",str);
        free_pages();
        if(str[0]=='q') break;
        char* end;
        tg_mem = strtoul(str,&end,10);
        tg_mem *= unit_expand(*end);
        visit_pages(tg_mem);
    }
    return 0;
}
