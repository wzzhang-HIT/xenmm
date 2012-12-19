/*
 * failed.c
 *
 *  Created on: Mar 1, 2011
 *      Author: moruo
 */

#include <stdio.h>
#include <stdlib.h>

#include "failed.h"

/*
 * Description: deal with errors, output error message
 *  and exist system with error code -1
 * Parameter: errstr( error message string )
 * Return: void
 */
void failed(const char *errstr)
{
    fprintf(stderr, "%s\n", errstr);
    exit(-1);
}

