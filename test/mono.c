/*=============================================================================
#     FileName: mono.c
#         Desc: mono memory benchmark
#               simply the original mono version from zhangdi
#       Author: xiehuc
#        Email: xiehuc@gmail.com
#     HomePage: 
#      Version: 0.0.1
#   LastChange: 2012-12-27 09:42:43
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
    ul low,high,target;
    if(argc!=3){show_help();return 0;}
    char* end;
    low = strtoul(argv[1],&end,10);
    low *= unit_expand(*end);
    high = strtoul(argv[2],&end,10);
    high *= unit_expand(*end);
    if(low>=high){
        fprintf(stderr,"low value must less than high value");
        return -1;
    }

    for(target = low;target<high;target+=ONE_PAGE*5){
        visit_pages(target);
    }
    for(target = high;target>=low;target-=ONE_PAGE*5){
        visit_pages(target);
    }
}
