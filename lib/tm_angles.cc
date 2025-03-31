// $RCSfile: tm_angles.cc,v $
// $Revision: 1.6 $
// $Date: 2009/07/23 20:57:10 $
//
// Utilities for manipulating angles as pol/gdec pairs,
// in either floating point or BCD-encoded formats.
//
// The GEODSS convention for declination is 0 at the north pole, increasing
// southward.  This differs from the normal RA/DEC convention which has
// 0 at the equator, increasing northward.  Hence the term "gdec" is used
// whereever the GEODSS declination convention is intended.
//
//****************************************************************************
// Copyright (c) 1995, California Institute of Technology.
// U.S. Government sponsorship is acknowledged.
//****************************************************************************
//
// Steve Groom, JPL
// 7/16/95

#include <stdio.h>
#include <sys/types.h>

#include <bcd.h>
#include <slalib.h>
#include <tm_angles.h>

/* convert angle value (0.0 - 359.9999) into BCD-encoded coarse/fine words */
static void
encode_angle(double ang, u_int &enc_c, u_int &enc_f)
{
    if ((ang < 0.0) || (ang >= 360.0))
    {
	fprintf(stderr,"encode_angle: error: value %f out of range\n",ang);
	enc_c = 0;
	enc_f = 0;
	return;
    }
    double dcoarse = (ang*10.0) + 0.0005;	// coarse part, plus rounding
    int icoarse = (int) (dcoarse);		/* drop fraction */
    double dfine = (dcoarse - icoarse)*1000.0;	/* pick up fraction */
    int ifine = (int) (dfine);
    enc_c = (i2bcd(icoarse) & 0x3fff);
    enc_f = (i2bcd(ifine) & 0x0fff);
}

/* convert BCD-encoded coarse/fine words into angle value (0.0 - 359.9999)*/
static void
decode_angle(u_int enc_c, u_int enc_f, double &ang)
{
    u_int fine;
    u_int coarse;

    fine = bcd2i(enc_f & 0x0fff);
    coarse = bcd2i(enc_c & 0x3fff);
    ang = (coarse + (fine/1000.0))/10.0;
    if ((ang<0) || ang>=360.0)
	fprintf(stderr,"decode_angle: warning: angle %f out of range\n",ang);
}

/* convert encoded pol/gdec positions into angles */
polgdec_ang::polgdec_ang(const polgdec_enc &pde)
{
    decode_angle(pde.pde_pol_c,pde.pde_pol_f,pda_pol);
    decode_angle(pde.pde_gdec_c,pde.pde_gdec_f,pda_gdec);
}

/* convert hour angle/declination to pol/gdec angles */
polgdec_ang::polgdec_ang(const hadec_ang &hda)
{
    pda_pol = fmod((360.0 + 180.0 - (hda.hda_ha*15.0)),360.0);
    pda_gdec = 90.0 - hda.hda_dec;
}

/* convert ra/dec to pol/gdec angles at sid time lst */
polgdec_ang::polgdec_ang(const radec_ang &rda, double lst)
{
    double ha = fmod((lst - rda.rda_ra + 24.0),24.0);
    pda_pol = fmod(360.0 + 180.0 - (ha*15.0),360.0);
    pda_gdec = 90.0 - rda.rda_dec;
}

polgdec_ang::polgdec_ang(const radec_ang_r &rda, double lst)
{
	double rah = ((rda.rda_ra*180)/PI)/15;
	double decd = (rda.rda_dec*180)/PI;

	double ha = fmod((lst - rah + 24.0),24.0);
	pda_pol = fmod(360.0 + 180.0 - (ha*15.0),360.0);
	pda_gdec = 90.0 - decd;
}

/* convert pol/gdec angles into BCD encoded positions */
polgdec_enc::polgdec_enc(double pol, double gdec)
{
    encode_angle(pol,pde_pol_c,pde_pol_f);
    encode_angle(gdec,pde_gdec_c,pde_gdec_f);
}

/* convert pol/gdec angles into BCD encoded positions */
polgdec_enc::polgdec_enc(const polgdec_ang &pda)
{
    encode_angle(pda.pda_pol,pde_pol_c,pde_pol_f);
    encode_angle(pda.pda_gdec,pde_gdec_c,pde_gdec_f);
}

/*
 * convert pol/gdec and site latitude into local azmuth/elevation (degrees)
 */
azel_ang::azel_ang(site &tmsite, const polgdec_ang &pda)
{
    double sgdec = sin(pda.pda_gdec*RPD);
    double cgdec = cos(pda.pda_gdec*RPD);
    double spol = sin(pda.pda_pol*RPD);
    double cpol = cos(pda.pda_pol*RPD);

    double asin_elev = cgdec*tmsite.slat() - cpol*sgdec*tmsite.clat();
    if (asin_elev > 1.0) asin_elev = 1.0;
    else if (asin_elev < -1.0) asin_elev = -1.0;

    el = asin(asin_elev)*DPR;

    double east = -spol*sgdec;
    double north = cgdec * tmsite.clat() + cpol*sgdec*tmsite.slat();
    az = fmod((atan2(east,north)*DPR + 360.0),360.0);	// map to 0 - 359.9999
}

/*
 * convert ha/dec and provided latitude into local az/el (degrees)
 */
void azel_ang::ha2azel(const double& lat, const hadec_ang& hda) {
    // use slalib function, convert degrees into radians first
    slaDe2h((hda.hda_ha*M_PI*15)/180, (hda.hda_dec*M_PI)/180,
	    (lat*M_PI)/180, &az, &el);
    // az and el are calced but in units of radians
    az = (az*180)/M_PI;
    el = (el*180)/M_PI;
}

azel_ang::azel_ang(const double& lat, const hadec_ang& hda) {
    ha2azel(lat, hda);
}

/*
 * convert ha/dec and site latitude into local az/el (degrees)
 */
azel_ang::azel_ang(site &tmsite, const hadec_ang &hda) {
    ha2azel(tmsite.slat(), hda);
}

/* convert pol/gdec to hour angle/declination */
hadec_ang::hadec_ang(const polgdec_ang &pda)
{
    hda_ha = fmod(((360.0 - pda.pda_pol + 180)/15.0),24.0);
    hda_dec = 90.0 - pda.pda_gdec;
}

/* convert RA/DEC and local sidereal time LST (in hours)
 * into hour angle/declination.
 */
hadec_ang::hadec_ang(const radec_ang &rda, double lst)
{
    hda_ha = fmod((lst - rda.rda_ra + 24.0),24.0);
    hda_dec = rda.rda_dec;
}

/* convert hour angle/declination and local sidereal time LST (in hours)
 * into right ascension/declination
 */
radec_ang::radec_ang(const hadec_ang &hda, double lst)
{
    rda_ra = fmod((lst - hda.hda_ha + 24.0),24.0);
    rda_dec = hda.hda_dec;
}
radec_ang::radec_ang(const polgdec_ang &pda, double lst)
{
    double ha = fmod(((360.0 - pda.pda_pol + 180)/15.0),24.0);
    rda_ra = fmod((lst - ha + 24.0),24.0);
    rda_dec = 90.0 - pda.pda_gdec;
}

radec_ang::radec_ang(const radec_ang_r &rda)
{
	rda_ra = ((rda.rda_ra*180)/PI)/15;
	rda_dec = (rda.rda_dec*180)/PI;
}

/* this set of contructors deals with changing RA/DEC coords from
 * hours/degrees to radians.
 */
radec_ang_r::radec_ang_r(double r, double d)
{
	rda_ra = ((r*15)*PI)/180;
	rda_dec = (d*PI/180);
}

radec_ang_r::radec_ang_r(const hadec_ang &hda, double lst)
{
	rda_ra = fmod((lst - hda.hda_ha + 24.0),24.0);
	rda_ra = (rda_ra*15*PI)/180;

	rda_dec = (hda.hda_dec*PI)/180;
}

radec_ang_r::radec_ang_r(const polgdec_ang &pda, double lst)
{
	double ha = fmod(((360.0 - pda.pda_pol + 180)/15.0),24.0);
	rda_ra = fmod((lst - ha + 24.0),24.0);
	rda_ra = (rda_ra*15*PI)/180;
	rda_dec = 90.0 - pda.pda_gdec;
	rda_dec = (rda_dec*PI)/180;
}

