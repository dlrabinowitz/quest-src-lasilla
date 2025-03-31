#ifndef _TM_FOCUS_H_
#define _TM_FOCUS_H_
// tm_focus.h 
// @(#)tm_focus.h	1.3 25 Oct 1995
//
// Utilities for converting GEODSS focus position (0-2047) to focus
// position word.
// Steve Groom, JPL
// 5/16/95

// The 11-bit focus value is encoded with one empty (unused) bit between
// each three bits in the value.  This expands the 11-bit value out to 16
// bits, with the high bit of each nibble unused.
//
// Output word format:
//	output bit:	pointing data bit:
//	15		1=on 0=off
//	14		unused
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

extern void encode_focus(int setting, u_int &focusword);

#endif /* _TM_FOCUS_H_ */
