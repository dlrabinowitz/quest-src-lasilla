// $RCSfile: tm_focus.cc,v $
// $Revision: 1.6 $
// $Date: 2009/07/23 20:57:10 $
//
// Utilities for converting focus setting to the
// encoded form expected by the GEODSS telescope mount.
//
//****************************************************************************
// Copyright (c) 1995, California Institute of Technology.
// U.S. Government sponsorship is acknowledged.
//****************************************************************************
//
// Steve Groom, JPL
// 5/16/95

#include <stdio.h>
#include <sys/types.h>
#include <math.h>

#include <tm_focus.h>

// convert focus setting into encoded focus position word
void
encode_focus(int fset, u_int &focus_enc)
{
    // 360.0 is first mapped back to 0 if necessary
    fprintf(stderr,"encode_focus: setting %d\n",fset);
    focus_enc =  (fset & 00007)       |	// these masks are octal!
    	       ((fset & 00070) << 1) |
    	       ((fset & 00700) << 2) |
    	       ((fset & 03000) << 3) |
	       (0x8000);			// high bit to turn on focus
    fprintf(stderr,"encode_focus: focus setting %d (0%o), enc 0x%x\n",
	fset,fset,focus_enc);
}
