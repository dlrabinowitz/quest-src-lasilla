#ifndef _TM_DOME_H_
#define _TM_DOME_H_
// tm_dome.h 
// @(#)tm_dome.h	1.3 25 Oct 1995
//
// Utilities for converting azimuth angle (degrees) to encoded dome
// position word.
// Steve Groom, JPL
// 5/11/95

// Reference: GEODSS subroutine IEUDME
//
// Azimuth is mapped into range 0.0-360.0 then converted to dome
// pointing units (0-4095).  The 12-bit pointing value is then encoded
// with one empty (unused) bit between each three bits in the value.
// This expands the 12-bit value out to 16 bits, with the high bit of
// each nibble unused.
//
// Output word format:
//	output bit:	pointing data bit:
//	15		not used
//	14		bit 11
//	13		bit 10
//	12		bit 9
//	11		unused
//	10		bit 8
//	 9		bit 7
//	 8		bit 6
//	 7		unused
//	 6		bit 5
//	 5		bit 4
//	 4		bit 3
//	 3		unused
//	 2		bit 2
//	 1		bit 1
//	 0		bit 0
//

extern double calc_dome_azm(double mount_azimuth, double mount_elevation);
extern void encode_dome(double azimuth, u_int &domeword);

#endif /* _TM_DOME_H_ */
