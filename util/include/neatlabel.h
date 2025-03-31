/*
 * $RCSfile: neatlabel.h,v $
 * $Revision: 1.6 $
 * $Date: 2009/07/23 20:57:10 $
 *
 * Declarations of routines to write VICAR labels to NEAT image files.
 *
 * Steve Groom, JPL
 * 7/15/95
 */

#ifndef _NEATLABEL_H_
#define _NEATLABEL_H_

#ifdef __cplusplus
#define EXTERN extern "C"
#else
#define EXTERN extern
#endif

#define NEAT_VIC_MIN_LABEL_SIZE	2048	/* initial label min size */

struct neat_vic_prop_info
{
    /* These fields describe the CCD device, not the image data.
     * The image size is defined in the VICAR system label (NL=rows,NS=cols).
     */
    int ccd_cols;	/* physical size of CCD, cols, including deadzone */
    int ccd_rows;	/* physical size of CCD, rows, including deadzone */
    int img_scol;	/* starting column of active area on CCD */
    int img_srow;	/* starting row of active area on CCD */
    int img_cols;	/* size of CCD active area, cols */
    int img_rows;	/* size of CCD active area, rows */
    int cols;		/* size of image readout from CCD, cols */
    int rows;		/* size of image readout from CCD, rows */

    char *descramstr;
    char *objectstr;
    char *datestr;
    char *timestr;
    double exptime;
    double obs_ra_hrs;
    double obs_ra_min;
    double obs_ra_sec;
    double obs_dec_deg;
    double obs_dec_min;
    double obs_dec_sec;
    int rowbin;
    int colbin;
    int refpix_col;
    int refpix_row;
    char *lststr;
    double hours_west;
    char *observerstr;
    double temperature1;
    double temperature2;
    double temperature3;
    char *shutterstr;		/* OPEN, CLOSED, or NONE */
    char *sequencestr;
    int frameid;
    char *rawfilestr;
    char *darkfilestr;

    char *observatory;
    char *telescope_id;
    int bitpix;
    double col_pix_size;
    double row_pix_size;
    double focal_len;
    double plate_scale;
    int saturation;
    char *north_orient;
    char *east_orient;
    char *sensor_type;
    char *sensor_info;
    double gain;		/* electrons per adu (ccds) */
    double imgunit;		/* image units (adu's etc) */
    double obs_lat;		/* north lat, deg */
    double obs_lon;		/* east lon, deg */
    double obs_alt;		/* elevation, meters */
};

struct neat_vic_hist_info
{
    char *taskname;
    char *username;
    char *datestr;
    int dnoffset;
    double focuspos;
    char *focusstr;
    char *polstr;
    char *decstr;
};

EXTERN int neat_vicar_sys_label(char *lblbuf, int lblsize, int rows, int cols);
EXTERN int neat_vicar_prop_label(char *lblbuf, struct neat_vic_prop_info *nvp);
EXTERN int neat_vicar_hist_label(char *lblbuf, struct neat_vic_hist_info *nvh);

#endif /* _NEATLABEL_H_ */
