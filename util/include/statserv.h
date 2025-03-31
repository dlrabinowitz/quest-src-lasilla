/*
 * soctest.h
 */
#ifndef _STATSERV_H_
#define _STATSERV_H_

#include <string.h>
#include <point.h>

#define STATPORT	9600

void print_smgr(struct schedmgr_status *ss, struct obsmgr_status *os,char* buf);

#endif // _STATSERV_H_
