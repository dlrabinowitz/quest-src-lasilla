#ifndef _DMS_H_
#define _DMS_H_
/* dms.h
 * @(#)dms.h	1.3 25 Oct 1995
 *
 * prototypes for degrees/minutes/seconds conversion routines
 *
 * Steve Groom, JPL
 * 7/16/95
 */

#ifdef __cplusplus
#define EXTERN extern "C"
#else
#define EXTERN extern
#endif

EXTERN void degdms(double d, int *di, int *mi, double *s);
EXTERN char *degdmsstr(double d, char *buf);
EXTERN void hrhms(double h, int *hi, int *mi, double *s);
EXTERN char *hrhmsstr(double h, char *buf);
EXTERN int hms_to_h(char *hms,double *h);
EXTERN int dms_to_d(char *dms,double *d);
EXTERN int hms_to_h_nocolon(char *hms,double *h);
EXTERN int dms_to_d_nocolon(char *dms,double *d);

#endif /* _DMS_H_ */
