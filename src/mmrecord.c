#include "mmrecord.h"
#include <time.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

static int max_idx = 0;

static void get_max_idx()
{
    char path[32];
    time_t t = time(NULL);
    struct tm tm_;
    int idx;
    localtime_r(&t, &tm_);
    strftime(path, sizeof(path), "%y_%m_%d_", &tm_);

    DIR* dir = opendir("./");
    struct dirent* ep;
    if(dir == NULL){
        perror(__FUNCTION__);
        return;
    }

    while((ep = readdir(dir))){
        if(!strncmp(path,ep->d_name,strlen(path))){
            sscanf(ep->d_name,"%*d_%*d_%*d_%d",&idx);
            if(idx>max_idx) max_idx = idx;
        }
    }
    closedir(dir);
    max_idx ++ ;
}

void record_begin(Domain* d)
{
    char path[128];
    struct tm tm_;
    time_t t = time(NULL);
    localtime_r(&t, &tm_);

    if(max_idx == 0)
        get_max_idx();

    strftime(path, sizeof(path), "%y_%m_%d_", &tm_);
    snprintf(path+strlen(path),sizeof(path)-strlen(path),"%d_[%d].log",max_idx,d->id);
    FILE* f = fopen(path,"w");
    if(f == NULL){
        perror(__FUNCTION__);
        return;
    }
    d->record = f;

    fprintf(f,"recorded by mmserver domain:[id:%d time:%s]\n",d->id,ctime(&t));
    fprintf(f,"time\t\ttg_mem\t\tuse_mem\t\tfree_mem\t\t\n");
}

void record_end(Domain* d)
{
    fclose(d->record);
    d->record = NULL;
}

void record_mem(Domain* d)
{
    if(!d || !d->record) return;
    char tstr[32];
    time_t t = time(NULL);
    struct tm *tm_ = localtime(&t);
    strftime(tstr, sizeof(tstr), "%X", tm_);

    fprintf(d->record,"%s\t\t%llu\t\t%llu\t\t%llu\t\t\n",tstr,d->tg_mem,d->tg_mem-d->free_mem,d->free_mem);
}