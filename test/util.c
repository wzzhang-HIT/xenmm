#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>

unsigned long rand_ul()
{
    unsigned long ret;
    int rand_stream = open("/dev/urandom",O_RDONLY);
    read(rand_stream, &ret, sizeof(ret));
    close(rand_stream);
    return ret;
}
int unit_expand(char u)
{
    switch(u){
        case 'k':
        case 'K':
            return 1;
        case 'm':
        case 'M':
            return 1024;
        case 'G':
            return 1024*1024;
        default:
            return 1;
    }
}
static void * trunks[MAX_PAGES];
static ul page_num = 0;
void visit_pages(ul size)
{
    ul new_pages = size / 4;
    int i;
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
            read(rand_stream, trunks[i], ONE_PAGE);
        }
    }
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
