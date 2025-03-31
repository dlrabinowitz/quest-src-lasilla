#ifndef _BCD_H_
#define _BCD_H_
/* bcd.h
 * @(#)bcd.h	1.3 25 Oct 1995
 *
 * Conversion to/from the BCD format used by the GEODSS telescope mount.
 * Steve Groom, JPL
 * July, 1995
 */

#include <sys/types.h>
extern u_int i2bcd(u_int ival);
extern u_int bcd2i(u_int bcdval);
#endif  /* _BCD_H_ */
