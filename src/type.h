/**
 * global definitions
 * author : xiehuc<xiehuc@gmail.com>
 *
 */
#ifndef TYPE_H_
#define TYPE_H_

#include <sys/queue.h>
#include <stdlib.h>
#include <stdio.h>

//this define the accuracy of memory adjust
//that is if adjust range less than accuracy
//we ignore it.
//value example 10*1024 -> 10MB
#define ACCURACY 0*1024
//this is tax rate as well as tau
#ifndef TAX_RATE
#define tax_rate 0.75
#else
#define tax_rate TAX_RATE
#endif

typedef unsigned int uint;
typedef unsigned long long mem_t;
typedef unsigned long ulong;

typedef struct Domain0 {
    mem_t tot_mem;
    mem_t free_mem;
    unsigned int page_size;
    LIST_HEAD(,Domain) domainu;
}Domain0;

extern Domain0 domain0;

typedef struct Domain {
    uint id;
    mem_t tot_mem;          ///< it is from guest perspective see total memory
    mem_t free_mem;         ///< it is from guest perspective see free memory
    /** target memory: 
     * it is from host perspective see allocated memory.
     * note tg_mem > tot_mem
     */
    mem_t tg_mem;           
    mem_t max_mem;          ///< it is from host perspective see maxmium memory setting

    FILE* record;

    LIST_ENTRY(Domain) entries;
}Domain;

typedef enum {
    MM_OK,
    MM_FAILED
}MMRetCode;

typedef struct MemInfo {
    uint id;
    mem_t d_max_mem;
    mem_t d_cur_mem;
    mem_t tot_mem;
    mem_t free_mem;
}MemInfo;

void* malloc0(size_t sz);

void domain0_init();
Domain* get_domain(uint id);

#endif /* GLOBAL_H_ */
