#ifndef _TM_RATE_H_
#define _TM_RATE_H_
// tm_rate.h 
// @(#)tm_rate.h	1.4 25 Oct 1995
//
// Utilities for representing pol/gdec rates in pairs as floating point
// values or BCD-encoded words.
// All rates here are stored in degrees per second.
// Steve Groom, JPL
// 5/9/95

// BCD format details:
//	 TBD

#include <sys/types.h>
#include <math.h>
//#include <neat/site.h>
#include <mathconst.h>

#define MAXRATE		999.9999	// largest representable rate value

class polgdec_rate_val;			// forward reference

class polgdec_rate_enc {
public:
    u_int pdre_pol_c;
    u_int pdre_gdec_c;
    u_int pdre_pol_f;
    u_int pdre_gdec_f;

    // Constructors
    polgdec_rate_enc(void)
    {
	pdre_pol_c = pdre_gdec_c = pdre_pol_f = pdre_gdec_f = 0;
    }
    polgdec_rate_enc(double rate_pol, double rate_gdec);
    polgdec_rate_enc(const polgdec_rate_val &pdrv);
};

class polgdec_rate_val {
public:
    double pdrv_pol;
    double pdrv_gdec;

    // Constructors
    polgdec_rate_val(void)
    {
	pdrv_pol = pdrv_gdec = 0.0;
    }
    polgdec_rate_val(double rate_pol, double rate_gdec)
    {
	pdrv_pol = rate_pol;
	pdrv_gdec = rate_gdec;
    }
    polgdec_rate_val(const polgdec_rate_enc &pdre);
};
#endif /* _TM_RATE_H_ */
