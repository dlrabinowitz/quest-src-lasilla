//
// $RCSfile: mountctl.cc,v $
// $Revision: 1.6 $
// $Date: 2009/07/23 20:57:11 $
// 
// Telescope mount manual control program.
// Allows user to interactively command the telescope mount.
// Commands which are understood are all those supported by the
// telescope::telinterp() function.
//
//****************************************************************************
// Copyright (c) 1995, California Institute of Technology.
// U.S. Government sponsorship is acknowledged.
//****************************************************************************
//
// Steve Groom, JPL
// 10/27/95
 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>

#include <telmount.h>
#include <neattime.h>

#define PROGNAME "mountctl"
#define COM_PORT 1 /* com_port TCS uses to take remote commands */

#define VERBOSE 0

int main(int argc, char *argv[])
{
    telmount *t;
    char command[1024];

    // setup mount
    if(VERBOSE){fprintf(stderr,"initializing mount\n");fflush(stderr);}

    t = new telmount();
    if ((t == NULL) || t->fail)
    {
	fprintf(stderr,"%s: mount initialization failed\n",argv[0]);fflush(stderr);
	if (t != NULL) delete t;
	exit(1);
    }

    if(VERBOSE){fprintf(stderr,"reading commands\n");fflush(stderr);}
    // main processing loop
    while(!feof(stdin))
    {
	printf("> "); fflush(stdout);
	if (fgets(command,sizeof(command),stdin) == NULL)
	    break;
        if(VERBOSE){fprintf(stderr,"next command %s\n",command);fflush(stderr);}
	command[sizeof(command)-1] = '\0';
	if (strlen(command)>1)
	{
            if(VERBOSE){fprintf(stderr,"executing next command \n");fflush(stderr);}
	    t->interp(fileno(stdout),command);
	}
    }
//  delete t;
    exit(0);
}
