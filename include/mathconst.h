#ifndef _MATHCONST_H
#define _MATHCONST_H
/* mathconst.h
 * @(#)mathconst.h	1.4 22 Nov 1995
 *
 * some useful mathematical constants
 */

#ifndef PI
#define PI 3.1415926535897932384626
#endif

#define RPD   0.01745329251994329577	/*  Radians Per Degree = PI/180 */
#define DPR  57.295779513082320876798	/* Degrees Per Radian = 180/PI */

/* Ratio of sidereal day length to solar day length.
 * OK, so this is more of an astronomical value than a mathematical
 * constant, but this file seemed like the best place to put it.
 */
#define SID_DAY	.99726956633		/* Astronomical Almanac 1995, B6 */

#endif /* _MATHCONST_H */
