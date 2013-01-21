#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>

#if RECORD_SWAP
#else
//empty define
#define record_swap()
#define close_swap()
#endif



unsigned long rand_ul()
{
    unsigned long ret;
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
    ul new_pages = size / 4;
    //page_num = size / 4;
    int i,times;
#if ! LOW_CPU
    int rand_stream = open("/dev/urandom",O_RDONLY);
    if(!rand_stream){
        perror("failed to open random stream");
        return;
    }
#endif
    //memset(trunks,sizeof(trunks),0);
    for(i=page_num;i<page_num+new_pages;i++){
        trunks[i] = malloc(ONE_PAGE);
        memset(trunks[i],ONE_PAGE,0);
        if(trunks[i] == NULL){
            perror("malloc failed");
            exit(-1);
        }
    }


#if ! LOW_CPU
    close(rand_stream);
#endif
    page_num += new_pages;
    flush_pages();
}
#if LOW_CPU
void flush_pages()
{
    int times,i;
    for(times=0;times<ITERATE_TIMES;times++){
        struct timespec tm_,rem_;
        tm_.tv_sec = 0;
        tm_.tv_nsec = NANO_SLEEP_TIME;
        nanosleep(&tm_,&rem_);
        for(i=0;i<page_num;i++){
            memset(trunks[i],times,ONE_PAGE);
        }
    }
}
#else
void flush_pages()
{
    int times,i;
    int rand_stream = open("/dev/urandom",O_RDONLY);
    for(times=0;times<ITERATE_TIMES;times++){
        for(i=0;i<page_num;i++){
            read(trunks[i], ONE_PAGE, rand_stream);
        }
    }
    record_swap();
    close(rand_stream);
}
#endif
void free_pages(ul size)
{
    ul delta_pages = size / 4;
    int i;
    for(i=page_num-1;i>=page_num-delta_pages;i--){
        free(trunks[i]);
    }
    page_num -= delta_pages;
}

void free_all_pages()
{
    int i;
    for(i=0;i<page_num;i++){
        free(trunks[i]);
    }
    page_num = 0;
}
