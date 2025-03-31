#ifndef _ASTRODSP_H_
#define _ASTRODSP_H_
/* astrodsp.h
 * @(#)astrodsp.h	1.4 25 Oct 1995
 *
 * Definitions of DSP commands for SDSU camera controller 
 * Adapted from code developed by SDSU.
 * Steve Groom, JPL
 * 7/6/95
 */

/*
 * Board ID's
 */
#define ASTROBRD_HOST		0
#define ASTROBRD_IF		1
#define ASTROBRD_TIM		2
#define ASTROBRD_UTIL		3

/*
 * DSP command definitions.
 */


#define ASTROCMD_TIMEOUT	1
/*
 * Multibyte char constants for commanding the DSPs.
 * Assume the last char goes in the low order byte, high end padded with 0.
 * This is done this way since the old method of multibyte character
 * constants is COMPLETELY nonportable, and varies from compiler to compiler!
 * This is somewhat more portable, but still suffers on a machine with
 * a different byte order.
 */

/* pack a three ascii chars into an int */
#define _PACKCHR(a,b,c)	((((a)&0x000000ff)<<16) | \
			 (((b)&0x000000ff)<<8) |  \
			  ((c)&0x000000ff))
/* boot commands */
#define	ASTROCMD_LDA	_PACKCHR('L','D','A')
#define	ASTROCMD_RDM	_PACKCHR('R','D','M')
#define	ASTROCMD_RST	_PACKCHR('R','S','T')
#define	ASTROCMD_TDL	_PACKCHR('T','D','L')
#define	ASTROCMD_WRM	_PACKCHR('W','R','M')

/* sbus commands */
#define	ASTROCMD_CSR	_PACKCHR('C','S','R')
#define	ASTROCMD_ADR	_PACKCHR('A','D','R')
#define	ASTROCMD_BCR	_PACKCHR('B','C','R')
#define	ASTROCMD_Reset	_PACKCHR('R','e','s')

/* timing board commands */
#define	ASTROCMD_CLR	_PACKCHR('C','L','R')
#define	ASTROCMD_HGN	_PACKCHR('H','G','N')
#define	ASTROCMD_IDL	_PACKCHR('I','D','L')
#define	ASTROCMD_LGN	_PACKCHR('L','G','N')
#define	ASTROCMD_RAD	_PACKCHR('R','A','D')
#define	ASTROCMD_RDC	_PACKCHR('R','D','C')
#define	ASTROCMD_SBV	_PACKCHR('S','B','V')
#define	ASTROCMD_STP	_PACKCHR('S','T','P')
#define	ASTROCMD_TCK	_PACKCHR('T','C','K')
#define	ASTROCMD_TDC	_PACKCHR('T','D','C')
#define	ASTROCMD_PSK	_PACKCHR('P','S','K')
#define	ASTROCMD_RDL	_PACKCHR('R','D','L')

/* utility board commands */
#define	ASTROCMD_AEX	_PACKCHR('A','E','X')
#define	ASTROCMD_CSH	_PACKCHR('C','S','H')
#define	ASTROCMD_OSH	_PACKCHR('O','S','H')
#define	ASTROCMD_PEX	_PACKCHR('P','E','X')
#define	ASTROCMD_PON	_PACKCHR('P','O','N')
#define	ASTROCMD_REX	_PACKCHR('R','E','X')
#define	ASTROCMD_SEX	_PACKCHR('S','E','X')
#define	ASTROCMD_SYR	_PACKCHR('S','Y','R')
#define	ASTROCMD_TAD	_PACKCHR('T','A','D')
#define	ASTROCMD_TDA	_PACKCHR('T','D','A')
#define	ASTROCMD_TDG	_PACKCHR('T','D','G')
#define	ASTROCMD_PIX	_PACKCHR('P','I','X')

/* result codes */
#define	ASTRORES_DON	_PACKCHR('D','O','N')
#define	ASTRORES_ERR	_PACKCHR('E','R','R')
#define	ASTRORES_COK	_PACKCHR('C','O','K')
#define	ASTRORES_RST	_PACKCHR('R','S','T')
#define	ASTRORES_ACTUAL	-1		/* return data as result */
#define	ASTRORES_NONE	0		/* no result expected */

/*
 * Masks for DSP P, X, and Y memory spaces.
 */
#define ASTRODSP_MEM_P		0x00100000
#define ASTRODSP_MEM_X		0x00200000
#define ASTRODSP_MEM_Y		0x00400000

#define ASTRODSP_ADDR_MAX	0x4000
 
#endif
