/*
 * xenstattool.h
 *
 *  Created on: May 17, 2011
 *      Author: moruo
 */

#include <xenstat.h>

#ifndef XENSTATTOOL_H_
#define XENSTATTOOL_H_

/*
 * Description: Initialize xenstat lib
 * Parameters: xhandle: xenstat handler, cur_node: current host node
 * Return: void
 */
void initxenstat(xenstat_handle ** xhandle, xenstat_node ** cur_node);

/*
 * Description: Clean up Xenstat lib
 * Parameters: xhandle: xenstat handler, cur_node: current host node
 */
void cleanupxenstat(xenstat_handle ** xhandle, xenstat_node ** cur_node);

/*
 * Description: Get domains infomation, if some domain start
 *  after this program, it also can work properly
 * Parameters: xhandle: xenstore handler, cur_node: current host node
 *  total_mem, total memory size, free_mem: free memory size
 *  domainsinfo: domains' information, num_domains: number domains
 * Return: void
 */
void getdomainsinfo(xenstat_handle ** xhandle, xenstat_node ** cur_node,
		unsigned long * total_mem, unsigned long * free_mem,
		struct domaininfo * domainsinfo, unsigned int * num_domains);

#endif /* XENSTATTOOL_H_ */
