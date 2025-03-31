// $RCSfile: bcd.cc,v $
// $Revision: 1.6 $
// $Date: 2009/07/23 20:57:10 $
//
// utilities for converting between BCD and unsigned integer values.
//
//****************************************************************************
// Copyright (c) 1995, California Institute of Technology.
// U.S. Government sponsorship is acknowledged.
//****************************************************************************
//
// Steve Groom, JPL
// 5/9/95

#include <stdio.h>

#include <bcd.h>

/* convert unsigned integer (range 0 - 9999) into 4-digit bcd value */
u_int
i2bcd(u_int ival)
{
    register u_int result;
    register u_int digit;
    register int i;

    if (ival > 9999)
    {
	fprintf(stderr,
	    "i2bcd: error: value %d out of range for BCD conversion\n",ival);
	return 0;
    }

    result = 0;
    for (i=0;i<=3;i++)	/* pick off each BCD digit, OR into result */
    {
	digit = (ival%10) << (i<<2);
	result |= digit;
	ival /= 10;
    }
    return result;
}

/* convert 4-digit bcd value into an unsigned integer */
u_int
bcd2i(u_int bcdval)
{
    register u_int result;
    register u_int digit;
    register int i;

    result = 0;
    for (i=3;i>=0;i--)	/* convert each BCD digit, add into result */
    {
	digit = (u_int)((bcdval >> (i<<2)) & 0x000f);
	if (digit>9)
	{
	    fprintf(stderr,"bcd2i: error: bad BCD digit in 0x%x\n",bcdval);
	    return 0;
	}
	result = (result*10) + digit;
    }
    return result;
}
