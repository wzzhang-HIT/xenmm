#ifndef UTIL_H_H
#define UTIL_H_H
typedef unsigned long ul;

#define MAX_MB 1024
#define ITERATE_TIMES 50

#define ONE_PAGE 1024*4
#define MAX_PAGES MAX_MB*1024/4

//response time,that is 
//sleep RESP_TIME between two visit pages action.
#define RESP_TIME 2

int unit_expand(char u);
double rand_num();
void visit_pages(ul size);
void free_pages(ul size);
#endif
