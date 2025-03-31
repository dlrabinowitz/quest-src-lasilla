#ifndef _NEATUTIL_H_
#define _NEATUTIL_H_
/* neatutil.h
 * @(#)neatutil.h	1.7 12 Dec 1995
 *
 * some definitions NEAT utility routines
 *
 * Steve Groom, JPL
 * 6/5/95
 */

#include <sys/types.h>
#include <syslog.h>
#include <sys/time.h>
#include <sys/types.h>
extern void neat_openlog(char *ident);
extern pid_t neat_checkpid(char *pidfilename);
extern int neat_lockfile(char *filename);
extern int neat_unlockfile(char *filename,int lockfd);
extern pid_t neat_spawn(char *file, char *argv[], int pipefd[]);
extern int neat_mapnewfile(char *filename, int nbytes, int *fd, caddr_t *addr);
extern double neat_freespace_mb(char *dirname);

#ifdef __svr4__
extern int usleep(unsigned int useconds);
#endif /* __svr4__ */


#define MIN(a,b)	(((a) < (b)) ? (a) : (b))
#define MAX(a,b)	(((a) > (b)) ? (a) : (b))
#endif /* _NEATUTIL_H_ */
