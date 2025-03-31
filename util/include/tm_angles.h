#ifndef _TM_ANGLES_H_
#define _TM_ANGLES_H_
// tm_angles.h 
// @(#)tm_angles.h	1.6 01 Dec 1995
//
// Utilities for representing angles as pol/gdec pairs (either
// as floating point values or BCD-encoded words), or az/el pairs.
//
// Steve Groom, JPL
// 7/16/95


// All angles here are stored in degrees.
//
// POL/GDEC:
//	Convention used by GEODSS telescopes.  POL is angle 0-360 degrees,
//	representing eastward rotation around polar axis.
//	0 = "down", 90=west, 180 = "up", 270 = west.
//	GDEC is declination southward from pole; 0 = celestial north pole,
//	90 = equator.
//	The "polgdec_enc" class is the GEODSS telescope BCD-encoded version
//	of the pol/gdec angles.
// HA/DEC:
//	Astronomical Hour Angle/Declination.  HA measured in hours WEST
//	(one hour == 15 degrees) of observer's meridian.
//	Declination measured in degrees north of equator.
// RA/DEC:
//	Right Ascension/Declination.  RA measured in hours EAST
//	of prime meridian.  Declination same as HA/DEC.
//

// POL/GDEC BCD format details:
// The representable range for the BCD format when combining both words
// is 0.0000 to 399.9999, but the largest value represented should be 359.9999,
// i.e. wrapping around to 360.0000 is out of range.
//
// The BCD format is split into "coarse" and "fine" words.
// The range for the coarse word is 0.0 - 359.9, and the range for the
// fine word is 0.0000 to 0.0999.  The bit values are as follows
// 	bit	coarse word bit value		fine word bit value
//	15: 	unused				unused
// 	14:	unused				unused
//	13:	200				unused
//	12:	100				unused
//	11:	80				.08
//	10:	40				.04
//	 9:	20				.02
//	 8:	10				.01
//	 7:	8				.008
//	 6:	4				.004
//	 5:	2				.002
//	 4:	1				.001
//	 3:	.8				.0008
//	 2:	.4				.0004
//	 1:	.2				.0002
//	 0:	.1				.0001

#include <sys/types.h>
#include <math.h>

#include <site.h>
#include <mathconst.h>

class polgdec_ang;	// forward reference
class azel_ang;	// forward reference
class hadec_ang;	// forward reference
class radec_ang;	// forward reference
class radec_ang_r;	// forward reference

// pol/gdec angle pair in BCD-encoded form
class polgdec_enc {
public:
    u_int pde_pol_c;
    u_int pde_gdec_c;
    u_int pde_pol_f;
    u_int pde_gdec_f;

    // Constructors
    polgdec_enc(void) { pde_pol_c = pde_gdec_c = pde_pol_f = pde_gdec_f = 0; }
    polgdec_enc(u_int pc, u_int pf, u_int dc, u_int df)
    {
	pde_pol_c = pc;
	pde_pol_f = pf;
	pde_gdec_c = dc;
	pde_gdec_f = df;
    }
    polgdec_enc(const polgdec_ang &pda);
    polgdec_enc(double pol, double gdec);

    // Operators
    int operator==(const polgdec_enc &p) const
	{return ((pde_pol_c == p.pde_pol_c) &&
		 (pde_gdec_c == p.pde_gdec_c) &&
		 (pde_pol_f == p.pde_pol_f) &&
		 (pde_gdec_f == p.pde_gdec_f)); }
};

// pol/gdec angle pair
class polgdec_ang {
public:
    double pda_pol;	// GEODSS "pol", degrees
    double pda_gdec;	// GEODSS declination, degrees

    // Constructors
    polgdec_ang(void) { pda_pol = pda_gdec = 0.0;}
    polgdec_ang(double p, double d) { pda_pol = p; pda_gdec = d; }
    polgdec_ang(const polgdec_enc &pde);
    polgdec_ang(const hadec_ang &hda);
    polgdec_ang(const radec_ang &rda,double lst);
	polgdec_ang(const radec_ang_r &rda, double lst);

    // Operators
    int operator==(const polgdec_ang &p) const
	{return ((pda_pol == p.pda_pol) && (pda_gdec == p.pda_gdec)); }
};

// azimuth/elevation angles
class azel_ang {
public:
    double az;
    double el;

    // Constructors
    azel_ang(void) { az = el = 0.0; }
    azel_ang(double a, double e) { az = a; el = e; }
    azel_ang(site &s, const polgdec_ang &pda); // pol/gdec to az/el
    azel_ang(site &s, const hadec_ang &hda);   // ha/dec to az/el
    azel_ang(const double &, const hadec_ang &);   // ha/dec to az/el, no site

    // Operators
    int operator==(const azel_ang &a) const
	{return ((az == a.az) && (el == a.el)); }
private:
    void ha2azel(const double&, const hadec_ang&);
};

// Hour angle / declination angle pair
class hadec_ang {
public:
    double hda_ha;	// Celestial hour angle, hours
    double hda_dec;	// Celestial declination, degrees

    // Constructors
    hadec_ang(void) { hda_ha = hda_dec = 0.0; }
    hadec_ang(double h, double d) { hda_ha = h; hda_dec = d; }
    hadec_ang(const polgdec_ang &pda);
    hadec_ang(const radec_ang &rda, double lst);

    // Operators
    int operator==(const hadec_ang &h) const
	{return ((hda_ha == h.hda_ha) && (hda_dec == h.hda_dec)); }
};

// Right Ascension / declination angle pair
class radec_ang {
public:
    double rda_ra;	// Right Ascension, hours
    double rda_dec;	// Celestial declination, degrees

    // Constructors
    radec_ang(void) { rda_ra = rda_dec = 0.0; }
    radec_ang(double r, double d) { rda_ra = r; rda_dec = d; }
    radec_ang(const hadec_ang &hda, double lst);
    radec_ang(const polgdec_ang &pda, double lst);
	radec_ang(const radec_ang_r &rda);

    // Operators
    int operator==(const radec_ang &r) const
	{return ((rda_ra == r.rda_ra) && (rda_dec == r.rda_dec)); }
};

// Right Ascension /declination angle pair - in radians
class radec_ang_r {
public:
    double rda_ra;	// Right Ascension, radians
    double rda_dec;	// Celestial declination, radians

    // Constructors
    radec_ang_r(void) { rda_ra = rda_dec = 0.0; }
    radec_ang_r(double r, double d);
    radec_ang_r(const hadec_ang &hda, double lst);
    radec_ang_r(const polgdec_ang &pda, double lst);

    // Operators
    int operator==(const radec_ang &r) const
	{return ((rda_ra == r.rda_ra) && (rda_dec == r.rda_dec)); }
};
#endif
