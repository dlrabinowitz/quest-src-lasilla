/*
 * $RCSfile: neatcam.h,v $
 * $Revision: 1.6 $
 * $Date: 2009/07/23 20:57:10 $
 *
 * Definition of the NEAT camera class.
 * Steve Groom, JPL
 * 7/12/95
 */

#ifndef _NEATCAM_H_
#define _NEATCAM_H_

#include <site.h>
#include <darkcat.h>
#include <telmount.h>

typedef struct 
{
    double mount_ra;
    double mount_dec;
    double mount_focus;
    double now;
    double t_expose;
} neatcam_mount_info;


#define LABEL_MIN_SIZE	2048
#define EXPOSURE_RESET_COUNT 16  // On Generation II SDSU boards, camera reset
			        //  is required every 64th exposure 
#define CLEAR_TIME 120.0        // seconds since last readout required before
				// CCD will be cleared
// Image acquisition mode:
enum acqmode {
     ACQ_DARK,		// dark current frame, shutter remains closed
     ACQ_FLAT,		// flat field, implementation TBD
     ACQ_SKY_STA,	// sky frame, stationary pointing
     ACQ_SKY_SID,	// sky frame, sidereal tracking
     ACQ_FOCUS,		// focus field, same as sidereal but different img label
     ACQ_YALE,		// Yale tracking
     ACQ_YALE_STOP,    	// Yale stop
};

class neatcam
{
private:
    int camfd;		// camera file descriptor
    darkcat dcat;	// information about dark images taken recently
    site cam_site;	// Lots of stuff about camera, site, etc

    int open();		// open/initialize camera
    void close();	// close camera device

public:
    int fail;
    int exposure_count;  // count of number of exposure since camera reset
    double time_last_read; // time of last readout

    // Constructors
    neatcam(const site &s);

    // Destructor
    ~neatcam();

    // Accessors
    int cam_rows() { return cam_site.cam_rows(); }
    int cam_cols() { return cam_site.cam_cols(); }
    double readrate() { return cam_site.cam_readrate(); }
 
    // Member functions
    int reset();	// close and then reopen  camera device
    int expose(telmount_client *tm, double ra, double dec, acqmode mode,
        double exposure, int rows, int cols, int nrbin, int ncbin,
        int timeflag, char *buf, double *time_done, neatcam_mount_info *m);
 //   int camera_initialize(int camfd, struct camreq *rq, double *t_expose1);
 //   int camera_readout(int camfd, char *rawfilename, struct camreq *rq);

    // read value from neatcam log from line starting with label    
    int read_neatcam_log(char *value, char *label); 

    // write string to neatcam log
    int write_neatcam_log(char *string, char *io_mode);  
						   

    // update private dark info
    void read_dcat();  // read in private dark catalogue from disk file
    void save_dcat();  // write private dark catalogue to disk file
    void update_darks(double exposure, char *filename, int timeflag, 
		     	double *time_done); 

    // assign filename to image
    int assign_name(char *filename, int timeflag, double *time_done, 
			acqmode mode);

    double ccdtemp(int temper);	// retrieve CCD temperature, deg C
 
    double cam_readout_time(); // return readout time in seconds
};
#endif /* _NEATCAM_H_ */
