#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>

typedef unsigned long memkb_t;
struct SwapInfo{
    memkb_t total;
    memkb_t freed;
};
static int prog_quit = 0;
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

static void interupt_server(int sig)
{
    prog_quit = 1;
}
int main(){
    signal(SIGINT,interupt_server);
    while(!prog_quit){
        record_swap();
        sleep(1);
    }
    close_swap();

}
