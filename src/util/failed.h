/*
 * failed.h
 *
 *  Created on: Mar 1, 2011
 *      Author: moruo
 */

#ifndef _FAILED_INCLUDE_
#define _FAILED_INCLUDE_

/*
 * Description: deal with errors, output error message
 *  and exist system with error code -1
 * Parameter: errstr( error message string )
 * Return: void
 */
void failed(const char *errstr);

#endif
