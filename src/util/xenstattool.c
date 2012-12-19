/*
 * xenstattool.c
 *
 *  Created on: May 17, 2011
 *      Author: moruo
 */

#include <stdio.h>
#include <string.h>

#include <xenstat.h>

#include "failed.h"
#include "global.h"

#include "xenstattool.h"

/*
 * Description: Initialize xenstat lib
 * Parameters: xhandle: xenstat handler, cur_node: current host node
 * Return: void
 */
void initxenstat(xenstat_handle ** xhandle, xenstat_node ** cur_node)
{
    if (*xhandle == NULL)
    {
        *xhandle = xenstat_init();
        if (*xhandle == NULL)
            failed("Failed to initialize xenstat library");
    }

    if (*cur_node == NULL)
    {
        *cur_node = xenstat_get_node(*xhandle, XENSTAT_ALL);
        if (*cur_node == NULL)
            failed("Failed to retrieve statistics from libxenstat");
    }
}

/*
 * Description: Clean up Xenstat lib
 * Parameters: xhandle: xenstat handler, cur_node: current host node
 */
void cleanupxenstat(xenstat_handle ** xhandle, xenstat_node ** cur_node)
{
    if (*cur_node != NULL)
        xenstat_free_node(*cur_node);

    if (*xhandle != NULL)
        xenstat_uninit(*xhandle);
}

/*
 * Description: Get domains infomation, if some domain start
 *  after this program, it also can work properly
 * Parameters: xhandle: xenstore handler, cur_node: current host node
 *  total_mem, total memory size, free_mem: free memory size
 *  domainsinfo: domains' information, num_domains: number domains
 * Return: void
 */

/**********************************************************************/
/* Idea: decide if there is the domain, this function is used, so     */
/* repair bug2[when the directballoon is running, a new domainU start */
/* there is error or xenstore read error output, and result in system */
/* exit with error. ]                                                 */
/**********************************************************************/
void getdomainsinfo(xenstat_handle ** xhandle, xenstat_node ** cur_node,
		unsigned long * total_mem, unsigned long * free_mem,
		struct domaininfo * domainsinfo, unsigned int * num_domains)
{
    unsigned int i;
    xenstat_domain *domains[MAX_DOMAINS];

    if (*cur_node != NULL)
        xenstat_free_node(*cur_node);

     *cur_node = xenstat_get_node(*xhandle, XENSTAT_ALL);
     if (*cur_node == NULL)
     {
         failed("Failed to retrieve statistics libxenstat");
     }

    /* Get total memory and free memory */
    *total_mem = xenstat_node_tot_mem(*cur_node) / 1024;
    *free_mem = xenstat_node_free_mem(*cur_node) / 1024;

    /* Get domains infomation  */
    *num_domains = xenstat_node_num_domains(*cur_node);

    for (i=0; i<(*num_domains); ++i)
    {
        domains[i] = xenstat_node_domain_by_index(*cur_node, i);
    }

    // Store domainsinfo in struct
    for (i=0; i<(*num_domains); ++i)
    {
        domainsinfo[i].domainid = xenstat_domain_id(domains[i]);
        strcpy(domainsinfo[i].domainname, xenstat_domain_name(domains[i]));
        domainsinfo[i].max_mem = xenstat_domain_max_mem(domains[i]) / 1024;
    }
}
