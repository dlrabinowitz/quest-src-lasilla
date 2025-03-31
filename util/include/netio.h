#ifndef _NETIO_H_
#define _NETIO_H_
/* netio.h
 * @(#)netio.h	1.3 25 Oct 1995
 *
 * definitions for network I/O utilities
 * Steve Groom, JPL
 * July 1995
 */

#ifdef __cplusplus
#define EXTERN extern "C"
#else
#define EXTERN extern
#endif

EXTERN int network_listen(int);
EXTERN void network_close(int);
EXTERN int network_accept(int);
EXTERN int network_connect(char *, int);
EXTERN int read_loop(int, unsigned char *, int);
EXTERN int write_loop(int, unsigned char *, int);

#endif  /* _NETIO_H_ */
