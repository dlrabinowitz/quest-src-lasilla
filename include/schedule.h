/*
 * $RCSfile: schedule.h,v $
 * $Revision: 1.6 $
 * $Date: 2009/07/23 20:57:10 $
 *
 * C++ class definitions used by observation scheduler
 *
 * Steve Groom, JPL
 * 6/5/95
 */
#ifndef _SCHEDULE_H_
#define _SCHEDULE_H_

#include <string.h>
#include <point.h>

#define SCHEDPORT	9500

#define SCHED_MISSINGFILENAME	"-"	/* used in place of missing filenames */

// Record of one observation of the sky.
class observation_info
{
friend class observation_request;
friend class observation_schedule;
private:
    double oi_time;		// actual time of observation
    int    oi_image_id;		// assigned image ID of observation
    char   oi_imgfile[NEAT_FILENAMELEN];	// image filename
    char   oi_darkfile[NEAT_FILENAMELEN];	// dark filename

public:
    // constructors
    observation_info() {};
    observation_info(double otime, char *imgfile, char *darkfile)
    {
	oi_time = otime;
	oi_image_id = 0;
	if (imgfile != NULL) (void) strcpy(oi_imgfile,imgfile);
	if (darkfile != NULL) (void) strcpy(oi_darkfile,darkfile);
    };
    observation_info(double otime, int image_id, char *imgfile, char *darkfile)
    {
	oi_time = otime;
	oi_image_id = image_id;
	if (imgfile != NULL) (void) strcpy(oi_imgfile,imgfile);
	if (darkfile != NULL) (void) strcpy(oi_darkfile,darkfile);
    };
};

// A request to observe a sky location some number of times
class observation_request
{
friend class observation_schedule;
private:
    int    or_id;		// request ID
    double or_ra;		// right ascension (J2000) to observe (hours)
    double or_dec;		// declination (J2000) to observe (degrees N+)
    double or_exposure;		// exposure time (secs) of each observation
    double or_interval;		// interval (secs) between observations
    int    or_count_wanted;	// number of observations at this position
    int    or_count_completed;	// number of them which have been completed
    double or_time_due;		// time when next observation is due
    double or_time_rise;	// estimate of when location will rise
    double or_time_set;		// estimate of when location will set
    int    or_shutter;		// 0 = dark, 1 = image
    int    or_drift;            // 0= tracking 1 = drift scan 
    int    or_binning;          // 1 = 1x1, 2 = 2x2, etc.
    observation_info *or_oi_list;	// collected observations for request
    observation_request *or_next;		// next request in list

public:

    int fail;

    // constructors
    observation_request() {}	// only used for status display 
    observation_request(double lat, double lon, double el,
	int id, double ra, double dec,
	double exposure, double intvl, int count, int shutterflag,
	int binning, double time_due);

    // destructor
    ~observation_request();

    // accessors
    int id()			{ return or_id; }
    double ra()			{ return or_ra; }
    double dec()		{ return or_dec; }
    double exposure()		{ return or_exposure; }
    double interval()		{ return or_interval; }
    int    count_wanted()	{ return or_count_wanted; }
    int    count_completed()	{ return or_count_completed; }
    double time_due()		{ return or_time_due; }
    double time_set()		{ return or_time_set; }
    double time_rise()		{ return or_time_rise; }
    void set_time_due(double t)	{ or_time_due = t; }
    int shutter()		{ return or_shutter; }
    int drift()               { return or_drift; }
    int binning()               { return or_binning; }

    // member functions
    int record_observation(observation_info &oi);
    observation_info *find_observation(int n);
};

// A schedule of how to execute a list of observation requests
class observation_schedule
{
private:
    observation_request *request_list;	// list of observation requests
    int next_id;

    // for logging of completed observations & observation requests
    int log_complete_observation(observation_request &obsor,observation_info &oi);
    int log_complete_request(observation_request &obsor);
    int log_rejected_request(char *line,char *comment);
    int log_rejected_request(observation_request &obsor,char *comment);
    int submit_anl_request(observation_request &obsor);

    double site_lat, site_lon;	// lat/lon of site, deg north/east
    double site_min_el;		// min elevation angle of observations, deg
    double cam_read_time;	// approximate camera readout time, seconds
    struct point cam_point_offsets[3];
    int runtype;

public:
    int fail;
    // constructors
    observation_schedule(void);	// Can't be used with select_next_request

    /* lat is degrees north, lon is deg east, el is minimum deg above horizon */
    observation_schedule(double lat, double lon, double el, double readtime,
			 struct point a_offset, struct point c_offset);

    // destructor
    ~observation_schedule();

    // member functions
    int load_file(char *filename);		// load requests from file
    void print();			// print requests
    					// record observation for request
    int record_observation(int id, observation_info &oi);
    					// select next observation to make
    observation_request *find_request(int id);

					// add observation request
    int submit_request(int id, double ra, double dec, double exposure,
	double intvl, int count, int shutterflag, int binning, double time_due);
    int submit_request(char *line);
    int cancel_request(int request_id);		// remove from schedule
						// can't complete request
    int reject_request(int id, char *why);
    int reject_request(observation_request &obsor, char *why);

    observation_request *select_next_request(double time_now);

    int checkpoint(char *filename);	// dump current state to file
    int restart(char *filename);	// restore current state from file
    int load_restart_line(char *line);	// load line from checkpoint file
    int snapshot(observation_request reqlist[], int list_max_size);
					// snapshot of schedule copied to array
    void purge(void);		// dump entire schedule to reject log
};
#endif // _SCHEDULE_H_
