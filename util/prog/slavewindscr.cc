/*
 * $RCSfile: slavewindscr.cc,v $
 * $Revision: 1.6 $
 * $Date: 2009/07/23 20:57:12 $
 * Test program for NEAT telescope mount.
 * Just checks to make sure we can take control
 *****************************************************************************
 * Copyright (c) 2000, California Institute of Technology.
 * U.S. Government sponsorship is acknowledged.
 *****************************************************************************
 *
 * Erik Hovland, JPL
 * $Date: 2009/07/23 20:57:12 $
 */

#include <iostream.h>
#include <strstream.h>
#include <fstream.h>
#include <iomanip.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <ctype.h>
#include <sys/time.h>
#include <signal.h>
#include <syslog.h>

#include <neatconf.h>
#include <telescope_controller.h>

int main(int argc, char *argv[]) {
    telescope_controller* telescope = new telescope_controller;

    int status = telescope->take_control();
    switch (status) {
    case 1:
        cout << "testmount: telescope ready" << endl;
        break;
    case 0:
        cout << "testmount: could not take control of telescope" << endl;
        break;
    case -1:
        cout << "testmount: could not communicate with telescope" << endl;
        break;
    default:
        cout << "testmount: unknown response from telescope: " << status
	     << endl;
        break;
    }
    if (status != 1) {
        exit(1);
    }
    status = telescope->slave_windscr();
    switch (status) {
    case 0:
        cout << "testmount: windscr slaved" << endl;
        break;
    case -1:
        cout << "testmount: could not communicate with telescope" << endl;
        break;
    default:
        cout << "testmount: unknown response from telescope: " << status
	     << endl;
        break;
    }

    if (status != 1) {
        exit(1);
    }

    exit(0);
}

