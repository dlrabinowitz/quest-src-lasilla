/* $RCSfile: loopio.c,v $
 * $Revision: 1.6 $
 * $Date: 2009/07/23 20:57:10 $
 *
 * Standard read/write routines enclosed in loops, so that partial
 * operations are properly completed.  This is necessary when operating
 * on pipes & sockets.
 *
 * Steve Groom 4/8/94
 *
 ******************************************************************************
 * Copyright (c) 1994, California Institute of Technology.
 * U.S. Government sponsorship under NASA Contract NAS7-918 is acknowledged.
 *****************************************************************************
 */

#ifdef VXWORKS
#include <vxWorks.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int read_loop(int fd, unsigned char *data, int nbytes)
{
    int nread;
    int count;

    nread = 0;
    while ((nbytes-nread) > 0) {
	count = read(fd,(char *) &data[nread],(nbytes-nread));
	if (count == -1) {
	    (void) fprintf(stderr,"read_loop: read failed\n");
	    perror("read");
	    return(-1);
	} else if (count == 0) {
	    /* EOF */
	    return(nread);
	}
	nread += count;
    }
    return(nread);
}

int write_loop(int fd, unsigned char *data, int nbytes)
{
    int nwrite;
    int count;

    nwrite = 0;
    while ((nbytes-nwrite) > 0) {
	count = write(fd,(char *) &data[nwrite],(nbytes-nwrite));
	if (count == -1) {
	    (void) fprintf(stderr,"write_loop: write failed\n");
	    perror("write");
	    return(-1);
	}
	nwrite += count;
    }
    return(nwrite);
}
