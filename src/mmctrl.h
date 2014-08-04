#ifndef MM_CTRL_H_H
#define MM_CTRL_H_H
#include "type.h"

void ctrl_init();
void ctrl_close();
/* fill all domain->max_mem */
void ctrl_read_domains_maxmem();
/* do real memory adjust, 
 * let d's total memory adjust to allocated
 */
void ctrl_update_domain_mem(Domain* d,mem_t allocated);

#endif
