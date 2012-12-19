/**
 * global definitions
 * author : xiehuc<xiehuc@gmail.com>
 *
 */
#ifndef TYPE_H_
#define TYPE_H_

typedef unsigned int uint;
typedef unsigned long long mem_t;

typedef struct Domain {
    uint id;
}Domain;

typedef enum {
    MM_OK,
    MM_FAILED
}MMRetCode;

typedef struct MemOf {
    mem_t tot_mem;
    mem_t free_mem;
}MemOf;

typedef struct MemInfo {
    uint id;
    mem_t max_mem;
    mem_t cur_mem;
    mem_t free_mem;
}MemInfo;

#endif /* GLOBAL_H_ */
