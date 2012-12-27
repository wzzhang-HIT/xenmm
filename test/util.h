#ifndef UTIL_H_H
#define UTIL_H_H
typedef unsigned long ul;

#define MAX_MB 4096
#define ITERATE_TIMES 50

#define ONE_PAGE 1024*4
#define MAX_PAGES MAX_MB*1024/4

int unit_expand(char u);
void visit_pages(ul size);
#endif
