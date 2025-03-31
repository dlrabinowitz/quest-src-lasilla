/***************************************************************************
 * $RCSfile: telescope_drv.cc,v $
 * $Revision: 1.8 $
 * $Date: 2010/10/12 14:37:27 $
 * Author: Erik Hovland (ehovland@huey.jpl.nasa.gov)
 *
 * driver program for serial port connection
 * between camera/telescope NEAT computer and TCU.
 *
 ***************************************************************************
 * Copyright (c) 2000, California Institute of Technology.
 * U.S. Government sponsorship is acknowledged.
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <syslog.h>
#include <string.h>
#include <iostream.h>
#define VERBOSE 0

#include <telescope_drv.h>
#include <quest_sockets.h>
int tel_drv_debug = 0;


/*
 * Send a command to the TCS and return with the telemetry stream. Uses a system call
 * to the TCS_PROGRAM to send the command. The TCS_PROGRAM packgages the command and
 * sends it to the TCS computer over a serial line. It then reads the continuous telemetry
 * stream coming back from the TCS computer over the same serial line and writes the latest 
 * telemetry record to standard output. In the system call, the standard output is
 * redirected to the TCS_STATUS_FILE which records all commands and telemetry responses.
 */
int telescope_io(char *cmd, char *response_string) {
    FILE *input;
    char string[1024];
    int n;

#if 1
    if(send_socket_command(cmd, response_string, TCS_STATUS_SRV_HOST,
                        TCS_STATUS_SRV_PORT, TCS_STATUS_SRV_TIMEOUT_SEC)<0){
       cerr << "telescope_io: unable to send command to TCS server" << endl;
       return(-1);
    }
    n=strlen(response_string);
    if(n>TCS_TELEMETRY_SIZE+1||n<TCS_TELEMETRY_SIZE){
      fprintf(stderr,
	"telescope_io: %s command: %s response string wrong length: %d\n%s\n",
					TCS_ERROR_CODE,cmd,strlen(response_string),response_string);
	fflush(stderr);
      return(-1);
    }
    else if (n>TCS_TELEMETRY_SIZE){
     *(response_string+TCS_TELEMETRY_SIZE)=0;
    }
#else
  if(strstr(cmd,TELESCOPE_CMD_NOCMD_STR)==NULL){
    if(VERBOSE){
       cerr << "telescope_io: forming system string with command " << cmd << endl;
       fflush(stderr);
    }
    /* send command to TCS (unless it is the STS command. These are unneccessary because the TCS
       continuously sends back status) */

    sprintf(string,"echo %s | %s > %s\n",cmd,TCS_PROGRAM,TCS_STATUS_FILE);

    if(VERBOSE){
       cerr << "telescope_io: system call : \n" << string << endl;
       fflush(stderr);
    }
    system(string);
  }
  else{
       cerr << "telescope_io: NOCOMMAND, skipping tcs call " << endl;
  }   

    /* open TCS_STATUS_FILE for reading */

    if(VERBOSE){
       cerr << "telescope_io: opening file " << TCS_STATUS_FILE << endl;
       fflush(stderr);
    }

    input=fopen(TCS_STATUS_FILE,"r");
    if(input==NULL){
      fprintf(stderr,"telescope_io: %s command: %s can't open TCS_STATUS_FILE %s\n",TCS_ERROR_CODE,cmd,TCS_STATUS_FILE);
      sprintf(response_string,"%s command: %s can't open TCS_STATUS_FILE %s\n",TCS_ERROR_CODE,cmd, TCS_STATUS_FILE);
      return(-1);
    }

    if(VERBOSE){
       cerr << "telescope_io: reading file " << TCS_STATUS_FILE << endl;
       fflush(stderr);
    }
    /* read the first line of the file. There should only be one line, 
       and it should be less than TEL_RESPONSE_SIZE long (expect TCS_TELEMETRY_SIZE)*/
 
    if(fgets(response_string,TEL_RESPONSE_SIZE,input)==NULL){
      fclose(input);
      fprintf(stderr,"telescope_io: %s command: %s can't read file %s\n",
							TCS_ERROR_CODE,cmd,TCS_STATUS_FILE);
      sprintf(response_string,"%s command: %s can't read file %s\n",TCS_ERROR_CODE,cmd, TCS_STATUS_FILE);
      return(-1);
    }

    fclose(input);
#endif

    if(VERBOSE){
       cerr << "telescope_io: response string is :\n " << response_string << endl;
       fflush(stderr);
    }

    if(VERBOSE){
       cerr << "telescope_io: done " << endl;
       fflush(stderr);
    }

    return (0);

}

