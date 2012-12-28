#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

double rand_num()
{
    double ret;
    int rand_stream = open("/dev/urandom",O_RDONLY);
    read(&ret,sizeof(ret),rand_stream);
    close(rand_stream);
    return ret;
}
int unit_expand(char u)
{
    if(u=='k'||u=='K') return 1;
    if(u=='m'||u=='M') return 1024;
    return 1;
}
static void * trunks[MAX_PAGES];
static ul page_num = 0;
void visit_pages(ul size)
{
    page_num = size / 4;
    int i,times;
    int rand_stream = open("/dev/urandom",O_RDONLY);
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
            read(trunks[i], ONE_PAGE, rand_stream);
        }
    }

    close(rand_stream);
}
void free_pages()
{
    int i;
    for(i=0;i<page_num;i++){
        free(trunks[i]);
    }
    page_num = 0;
}
