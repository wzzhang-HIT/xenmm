/*=============================================================================
#     FileName: random.c
#         Desc: random memory benchmark
#               simplify the original mono version from zhangdi
#       Author: xiehuc
#        Email: xiehuc@gmail.com
#     HomePage: 
#      Version: 0.0.1
#   LastChange: 2012-12-27 11:06:52
#      History:
=============================================================================*/

#include "util.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
//adjust this value to set max mb size of memory
//malloced
void show_help()
{
    const char* help=
        "useage: random low high\n"
        "useage: random low high last\n"
        "example: random 100M 500M\n";
        "\t run random from 100M to 500M last default 10s\n"
        "example: random 100M 500M 30s\n"
        "\t run random from 100M to 500M last 30s\n";
    printf("%s\n",help);
}
int main(int argc,char** argv)
{
    ul low,high,target,duration=20;
    if(argc<3||argc>4){show_help();return 0;}
    char* end;
    if(argc>=3){
        low = strtoul(argv[1],&end,10);
        low *= unit_expand(*end);
        high = strtoul(argv[2],&end,10);
        high *= unit_expand(*end);
    }
    if(argc>=4){
        duration = strtoul(argv[3],&end,10);
    }
    if(low>=high){
        fprintf(stderr,"low value must less than high value\n");
        return -1;
    }
    if(high>=MAX_MB*1024){
        fprintf(stderr,"high value must less than MAX_MB(%u)\n",MAX_MB);
        return -1;
    }

    printf("\n");
    time_t start = time(NULL);
    time_t ed;
    double rate;
    do{
        ed = time(NULL);
        srand(ed);
        rate = (double)rand()/RAND_MAX;
        target = (ul)(rate*(high-low)+low);
        printf(".");
        fflush(stdout);
        free_all_pages();
        visit_pages(target);
        sleep(1);
    }while(ed-start<duration);

    free_all_pages();
    printf("\n");
    return 0;
}
