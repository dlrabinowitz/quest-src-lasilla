/* @(#)netio.c	1.7 6/14/95
 * netio.c
 * Routines to implement basic socket-oriented network connections.
 *
 * Steve Groom 4/8/94
 * last update: 1/5/95
 *
 ******************************************************************************
 * Copyright (c) 1994-1995, California Institute of Technology.
 * U.S. Government sponsorship under NASA Contract NAS7-918 is acknowledged.
 *****************************************************************************
 */
#ifndef lint
/*
 *static char *sccsid = "@(#)netio.c	1.7 6/14/95";
 *static char *copyright = 
 *	"Copyright (c) 1994-1995, California Institute of Technology";
 */
#endif /* !lint */

/*
 * network_listen() - establish an incoming socket to receive connections
 * network_accept() - receive a connection to the socket returned
 *                    by network_listen()
 * network_connect() - make an outgoing connection to host/port
 */


#include <unistd.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

/* Establish an incoming socket to receive connections.
 * Returns listener socket for passing to network_accept(), or -1 on error.
 * The socket must be closed by the caller when no more connections
 * are wanted.
 */
int network_listen(int port)
{
    struct sockaddr_in sin;
    int len;
    int sock;
    int on;

    /* create socket */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket");
        return(-1);
    }

    /* reuse addresses (don't fail if another socket is already listening
     * in this same port, just supercede it).
     */
    on = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR,
            (char *) &on, sizeof(on)) == -1) {
        perror("setsockopt");
	(void) close(sock);
        return(-1);
    }

    /* bind socket to desired port */
    len = sizeof(sin);
    (void) memset((char *) &sin, 0, len);
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_family = AF_INET;
    sin.sin_port = htons((short) port);
    if (bind(sock, (struct sockaddr *) &sin, len) == -1) {
        perror("bind");
	(void) close(sock);
        return(-1);
    }

    /* tell socket to listen for connections */
    (void)fprintf(stderr,"waiting for connections on TCP port %d\n",port);
    if (listen(sock,1) == -1) {
        perror("listen");
	(void) close(sock);
        return(-1);
    }
    return(sock);
}

void network_close(int sock)
{
	(void) close(sock);
}

/* Accept a connection on a socket (as returned by network_listen())
 * Returns connected bidirectional socket to client, or -1 on error.
 * Should be closed by caller when connection is to be terminated.
 */
int network_accept(int sock)
{
    struct sockaddr_in sin;
    int len;
    int insock;
    
    /* accept connections */
    len = sizeof(sin);
    (void) memset((char *) &sin, 0, len);
    while (((insock = accept(sock, (struct sockaddr *) &sin, &len)) == -1) &&
	   (errno == EINTR));
    if (insock == -1) {
	perror("accept");
	return(-1);
    }
    /* verify address family of socket, not really required */
    if (sin.sin_family == AF_INET) {
        (void)fprintf(stderr,
	    "got connection from %s\n",inet_ntoa(sin.sin_addr));
    } else {
        (void)fprintf(stderr,
	    "strange socket family, sin_family = %d\n",sin.sin_family);
    }
    return(insock);
}

/* Make an outoing connection to host/port.  "host" can be a hostname
 * or dotted-quad IP address in string form.
 * Returns connected bidirectional socket on success, -1 on failure.
 */
int network_connect(char *host, int port)
{
    int len;
    struct sockaddr_in sin;
    struct hostent *hp;
    u_long laddr;
    int outchan;

    /* create the socket */
    outchan = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (outchan == -1) {
	perror("socket");
	return(-1);
    }

    /* see if the host was a numeric IP address */
    if (isdigit((int) host[0])) {
	/* use numeric IP address */
	laddr = inet_addr(host);
	if (laddr == (u_long) -1) {
	    (void)fprintf(stderr,"can't parse numeric IP address %s\n",host);
	    (void) close(outchan);
	    return(-1);
	}
    } else {
#ifdef VXWORKS
	laddr = hostGetByName(host);
	if (laddr == (u_long) ERROR) {
	    (void)fprintf(stderr,"%s: host unknown\n",host);
	    (void) close(outchan);
	    return(-1);
	}
#else /* !VXWORKS */
	/* lookup IP address for this host */
	hp = gethostbyname(host);
	if (hp == NULL) {
	    (void)fprintf(stderr,"%s: host unknown\n",host);
	    (void) close(outchan);
	    return(-1);
	}
	(void) memcpy((char *) &laddr, hp->h_addr_list[0], hp->h_length);
#endif /* !VXWORKS */
    }
    /* build socket address structure */
    len = sizeof(sin);
    (void) memset((char *) &sin, 0, len);
    sin.sin_addr.s_addr = laddr;
    sin.sin_family = AF_INET;
    sin.sin_port = htons((short) port);

    /* make the connection */
    if (connect(outchan, (struct sockaddr *) &sin, len) == -1) {
	/***
	perror("connect");
	(void)fprintf(stderr,"%s: connection failed\n",host);
	***/
	return(-1);
    }
    return(outchan);
}
