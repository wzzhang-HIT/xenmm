#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>

#if RECORD_SWAP
typedef unsigned long memkb_t;
struct SwapInfo{
    memkb_t total;
    memkb_t freed;
};
static int record_fd = -1;
static int proc_fd = -1;
static void record_swap()
{
    //reduce write times
    static int reduce = 0;
    reduce %= 3;
    if(reduce++!=0) return;

    char buf[4096];
    struct SwapInfo info = {0};
    char* ptr;
    time_t t = time(NULL);
    struct tm *tm_ = localtime(&t);
    if(record_fd==-1){
        snprintf(buf,sizeof(buf),"/proc/meminfo");
        proc_fd = open(buf,O_RDONLY);
        strftime(buf, sizeof(buf), "[swap]%y-%m-%d-%X.log", tm_);
        record_fd = open(buf,O_WRONLY|O_CREAT);
        snprintf(buf,sizeof(buf),"record in kb\ntime\t\ttotal\t\tused\t\tfree\n");
        write(record_fd,buf,strlen(buf));
    }
    lseek(proc_fd,0,SEEK_SET);
    size_t len = read(proc_fd,buf,sizeof(buf)-1);
    buf[len]='\0';
    ptr = strstr(buf,"SwapTotal:");
    ptr+=strlen("SwapTotal:");
    info.total = strtoul(ptr, NULL, 10);
    ptr = strstr(ptr,"SwapFree:");
    ptr+=strlen("SwapFree:");
    info.freed = strtoul(ptr, NULL, 10);
    strftime(buf,sizeof(buf),"[%X]\t\t",tm_);
    snprintf(buf+strlen(buf),sizeof(buf)-strlen(buf),"%lu\t\t%lu\t\t%lu\n",info.total,info.total-info.freed,info.freed);
    write(record_fd,buf,strlen(buf));
}
static void close_swap()
{
    close(record_fd);
    close(proc_fd);
    record_fd = proc_fd = -1;
}
#else
//empty define
#define record_swap()
#define close_swap()
#endif



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
        record_swap();
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
    close_swap();
}
