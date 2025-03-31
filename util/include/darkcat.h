#ifndef _DARKINFO_H_
#define _DARKINFO_H_
// darkinfo.h
// @(#)darkcat.h	1.2 05 Jan 1996
//
// Definition of "darkinfo" object,
// containing information about dark image frames
// taken recently.
//
// Steve Groom, JPL
// 1/5/96

// Information a camera "dark" image frame.
class darkinfo
{
friend class darkcat;
private:
    double di_exptime;			// exposure time in seconds
    double di_time_done;		// when it was taken (uxt)
    char di_filename[NEAT_FILENAMELEN];	// filename

public:
    // accessors
    double exptime()		{ return di_exptime; }
    double time_done()		{ return di_time_done; }
    char *filename()		{ return di_filename; }
};

// A searchable catalog of dark image frames
class darkcat
{
private:
    int ndarks;
    darkinfo dlist[NEAT_MAXDARKTIMES];

public:
    darkcat() { ndarks=0; }
    int add(double exptime, double time_done, char *filename); // add to catalog
    int load(char *filename);			// load catalog from file
    darkinfo *find(double exptime);		// locate entry in catalog
    int dump(char *filename);			// dump catalog to file
};

#endif // _DARKINFO_H_
