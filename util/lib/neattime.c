/*
 * $RCSfile: neattime.c,v $
 * $Revision: 1.6 $
 * $Date: 2009/07/23 20:57:10 $
 * Routines for manipulating various representations of time
 *
 * Formats supported:
 *	uxt	UNIX time, seconds past January 1, 1970.
 *		Used as the basic time representation, since that's what
 *		the system clock is running on.
 *	lst	Local Mean Sidereal Time.
 *	mjd	Modified Julian Date
 *
 *****************************************************************************
 * Copyright (c) 1995, California Institute of Technology.
 * U.S. Government sponsorship is acknowledged.
 *****************************************************************************
 *
 * Steve Groom, JPL
 * 7/16/95
 */

#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <math.h>

#include <neattime.h>

/* get_time_of_day
 * Read system clock, return seconds tm structure with month, day, year,
 * hour, minute, and second UT
 */
void
get_date_time(struct tm *tm)
{
    struct timeval tv;

    (void) gettimeofday(&tv,NULL);
    *tm = *gmtime(&(tv.tv_sec));
    return;
}

/* neat_gettime_utc
 * Read system clock, return seconds past 1/1/1970 UTC (i.e. UNIX time, uxt).
 * Includes available sub-second precision as fractional part.
 */
double
neat_gettime_utc()
{
    struct timeval tv;
    double t;

    (void) gettimeofday(&tv,NULL);
    t = tv.tv_sec + (tv.tv_usec/1000000.0);
    return t;
}

/* uxt_mjd
 * convert Unix time (UXT) to Modified Julian date
 */
double
uxt_mjd(double uxt)
{
    /* Unix time is seconds past 00:00:00 1/1/1970.
     * Julian date was UXT0 (2440587.5) then.
     * Modified Julian date was UXT0-MJD0 then.
     */

    double mjd;
    mjd = (UXT0-MJD0) + uxt/3600.0/24.0;
    return mjd;
}

/* uxt_mjd
 * convert Unix time (UXT) to Local Mean Sidereal Time
 */
double
uxt_lst(double uxt, double longitude)
{
    return mjd_lst(uxt_mjd(uxt),longitude);
}

/* mjd_uxt
 * convert Modified Julian Date mjd to Unix time
 */
double
mjd_uxt(double mjd)
{
    /* just the reverse of uxt_mjd(), above */
    double uxt;
    uxt = (mjd - (UXT0-MJD0)) * 3600.0 * 24.0;
    return uxt;
}

/* Convert modified Julian date mjd to Local Mean Sidereal Time (LMST)
 * at given EAST longitude (degrees).
 * From the Astronomical Almanac 1995, page B6
 * Returns LMST in fractional hours.
 */
double
mjd_lst(double mjd, double longitude)
{
    double t;
    double gmst;
    double lst;
    double ut;	/* hour of day */
    double jd;

    ut = (mjd - (int)mjd) * 24.0;	/* fractional part of day */
    jd = mjd + MJD0;
    t = (jd - J2000) / 36525.0;
    gmst = (24110.54841 + t * (8640184.812866 + t * (0.093104 - t * 6.2e-6)))
	/ 3600.0;

    /* we now have GMST in hours, correct for longitude*/

    lst = fmod((gmst + ut + longitude/15.0),24.0);
    if (lst<0.0) lst += 24.0;
    return lst;
}
