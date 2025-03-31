#ifndef _TELESCOPE_H_
#define _TELESCOPE_H_
/* telescope.h
 * @(#)telescope.h	1.5 04 Dec 1995
 *
 * Definition of the "telescope" class, i.e. the camera, telescope mount
 * and dome.
 *
 * Steve Groom, JPL
 * 7/13/95
 */

#include <telmount.h>
#include <neatcam.h>

class telescope
{
private:
    neatcam		*cam;	// the camera

    telmount_client	*telm;	// the telescope mount/dome manager

public:
    //
    int fail;
    pid_t child_pid;

    // Constructor
    telescope();

    // Destructor
    ~telescope();

    // Accessors
    int cam_rows() { return cam->cam_rows(); }
    int cam_cols() { return cam->cam_cols(); }

    // Member functions
    int stop();					// stop motion
    int stow();				// move mount/dome to "stow" position
    int point(double ra, double dec);	// move mount/dome to ra/dec J2000
    int point_pd(double pol, double gdec);	// move mount/dome to pol/gdec
    int sidtrack(double ra, double dec);	// sidereal track at ra/dec
    int sidtrack_pd(double pol, double gdec);	// sidereal track at pol/gdec
    int dome(double dome_azm);		// move dome (only) to azimuth
    int focus(double fset);			// set focus
    int getpos(double *ra, double *dec);          // read current ra/dec
    int getpos_pd(double *pol_p, double *gdec_p); // read current pol/gdec
    int safepos(double ra, double dec); // is it OK to move to ra/dec ?
    int safepos_pd(double pol, double gdec); // is it OK to move to ra/dec ?


    // image acquisition:
    int acquire(double ra, double dec, acqmode mode, double exposure,
	int rows, int cols, int nrbin, int ncbin, int timeflag,
	char *filename, double *time_done);

    // sawn cloned child process to expose and readout camera
    int background_expose(double ra, double dec, acqmode mode, double exposure,
	int rows, int cols, int nrbin, int ncbin, int timeflag,
	char *filename, double *time_done, neatcam_mount_info *m);

    // determine whether specified status is valid for spawned child process
    int child_wait(char *status_word, char *value, double timeout);
    int update_child_info (char *filename, double *time_done );

};
#endif /* _TELESCOPE_H_ */
