#ifndef _PRECESS_H_
#define _PRECESS_H_
/* precess.h
 * @(#)precess.h	1.3 25 Oct 1995
 *
 * Precession of RA/DEC coordinates
 *
 * Steve Groom, JPL
 * 7/16/95
 */

#ifdef __cplusplus
#define EXTERN extern "C"
#else
#define EXTERN extern
#endif

EXTERN void precess(double mjd1, double mjd2, double *ra, double *dec);

#endif /* _PRECESS_H_ */
