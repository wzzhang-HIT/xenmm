#ifndef UTIL_H_H
#define UTIL_H_H
typedef unsigned long ul;

#define LOW_CPU 1
#define RECORD_SWAP 1

//50 ms
#define NANO_SLEEP_TIME 300*1000*1000

#define MAX_MB 1024
#define ITERATE_TIMES 50

#define ONE_PAGE 1024*4
#define MAX_PAGES MAX_MB*1024/4


//response time,that is 
//sleep RESP_TIME between two visit pages action.
#define RESP_TIME 2

int unit_expand(char u);
unsigned long rand_ul();
void visit_pages(ul size);
void free_pages(ul size);
void flush_pages();
#endif
