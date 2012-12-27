#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int unit_expand(char u)
{
    if(u=='k'||u=='K') return 1;
    if(u=='m'||u=='M') return 1024;
    return 1;
}
void visit_pages(ul size)
{
    void * trunks[MAX_PAGES];
    ul page_num = size/4;
    int i,times;
    FILE* rand_stream = fopen("/dev/random","r");
    if(!rand_stream){
        perror("failed to open random stream");
        return;
    }
    memset(trunks,sizeof(trunks),0);
    for(i=0;i<page_num;i++){
        trunks[i] = malloc(ONE_PAGE);
        if(trunks[i] == NULL){
            perror("malloc failed");
            break;
        }
    }
    page_num = i;

    for(times=0;times<ITERATE_TIMES;times++){
        for(i=0;i<page_num;i++){
            fread(trunks[i], 1, ONE_PAGE, rand_stream);
        }
    }

    fclose(rand_stream);
    for(i=0;i<page_num;i++){
        free(trunks[i]);
    }
}
