/* precess.c
 * @(#)precess.c	1.3 21 Nov 1995
 * Precess RA/DEC coordinates from one epoch to another.
 * Does not account for nutation.
 *
 *****************************************************************************
 * Copyright (c) 1995, California Institute of Technology.
 * U.S. Government sponsorship is acknowledged.
 *****************************************************************************
 *
 * Steve Groom, JPL
 * 7/16/95
 */

/*
 *static char *sccsid="@(#)precess.c	1.3 21 Nov 1995";
 *static char *copyright=
 *   "Copyright (c) 1995, California Institute of Technology.\
 *   U.S. Government sponsorship is acknowledged.";
 */

#include <math.h>

#include <mathconst.h>
#include <precess.h>

/* "Rigorous" precession of ra/dec from epoch mjd1 to epoch mjd2.
 * RA is in hours, DEC in degrees.  Both are modified in place.
 * From the Astronomical Almanac 1995, B18
 */
void
precess(double mjd1, double mjd2, double *ra, double *dec)
{

    double ra1;		/* RA in degrees at MJD1 */
    double dec1;	/* DEC in degrees at MJD1 */
    double ra2;		/* RA in degrees at MJD2 */
    double dec2;	/* DEC in degrees at MJD2 */

    double t;		/* centuries from mjd2 to mjd1 */ 
    double zeta_a;
    double z_a;
    double theta_a;
    double A, B, C;

/*
 *   double cosdec1;
 *   double sindec1;
 *   double costheta_a;
 *   double sintheta_a;
 *   double cosra1z_a;
 */

    ra1  = (*ra * 15.0);	/* convert hours to radians */
    dec1 = (*dec);	/* convert degrees to radians */

    t 		= (mjd1 - mjd2) / 36525.0;
    zeta_a 	= 0.6406161*t + 0.0000839*t*t + 0.0000050*t*t*t;
    z_a		= 0.6406161*t + 0.0003041*t*t + 0.0000051*t*t*t;
    theta_a	= 0.5567530*t - 0.0001185*t*t - 0.0000116*t*t*t;

    A = sin(RPD*(ra1-z_a)) * cos(RPD*dec1);
    B = cos(RPD*(ra1-z_a)) * cos(RPD*theta_a) * cos(RPD*dec1) + 
	sin(RPD*theta_a) * sin(RPD*dec1);
    C = -cos(RPD*(ra1-z_a)) * sin(RPD*theta_a) * cos(RPD*dec1) +
	cos(RPD*theta_a) * sin(RPD*dec1);

    ra2 =  DPR*atan2(A,B) - zeta_a;

    if (C>1.0) C = 1.0;
    else if (C<-1.0) C = -1.0;
    dec2 = DPR*asin(C);

    *ra  = fmod(ra2/15.0+24.0,24.0);	/* convert radians to hours */
    *dec = dec2;		/* convert radians to degrees */
}


#ifdef MAIN
#include <neattime.h>
main()
{
    double ra;
    double dec;
    double mjd; 

    ra = 18.6155555555;			/* Vega 2000 */
    dec = 38.7833333333;		/* Vega 2000 */
    mjd = 50042;			/* 11/20/1995 */
    printf("mjd 2000: %lf  mjd now %lf\n",MJ2000,mjd);
    printf("J2000: ra %12.7lf  dec %12.7lf\n",ra,dec);
    precess(MJ2000,mjd,&ra,&dec);
    printf("now: ra %12.7lf  dec %12.7lf\n",ra,dec);

    ra = 18.6155555555;			/* Vega 2000 */
    dec = 38.7833333333;		/* Vega 2000 */
    mjd = 54000;			/* 9/22/2006 */
    printf("mjd 2000: %lf  mjd now %lf\n",MJ2000,mjd);
    printf("J2000: ra %12.7lf  dec %12.7lf\n",ra,dec);
    precess(MJ2000,mjd,&ra,&dec);
    printf("now: ra %12.7lf  dec %12.7lf\n",ra,dec);
}
#endif 
