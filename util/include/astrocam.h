/*
 * $RCSfile: astrocam.h,v $
 * $Revision: 1.6 $
 * $Date: 2009/07/23 20:57:09 $
 *
 * Prototypes for application interface to SDSU "astro" camera interface.
 *
 * Steve Groom, JPL
 * 7/6/95
 */

#ifndef _ASTROCAM_H_
#define _ASTROCAM_H_

#include <sys/types.h>

/* some trickiness so these functions can be called from both C and C++ */
#ifdef __cplusplus
#define EXTERN	extern "C"
#else
#define EXTERN	extern
#endif

#define CAM_RESET_RETRIES	10	/* times to retry camera reset - was 3*/

#define CAMENGSIZE	64

typedef struct camreq {
	int shutter;     /* open shutter 1=yes 0=no */
	double exposure; /* exposure time in seconds */
	int	activerows,activecols;	/* size of active area of CCD, pixels */
	int	deadrows,deadcols;	/* dead area surrounding active area, pixels */
	int	rows,cols;   /* center portion of active area to expose, even sizes! */
	int	nrbin,ncbin; /* binning size in row, col directions */
	int dnoffset;	/* amt to subtract from each raw pixel before storing */
	char *imgbuf;  /* pointer to user-allocated data area */
	char *imgbuf1;  /* pointer to user-allocated data area */
	char *imgbuf2;  /* pointer to user-allocated data area */
	char *imgbuf3;  /* pointer to user-allocated data area */
	int buflen;	     /* length of data area in bytes */
	int engdat[CAMENGSIZE];	/* returned engineering data about exposure */
	int shutter_bounce;
	int util_timer;
} camreq;

/*
 * Camera application-level routines
 * This is the preferred highest-level interface to the camera.
 */
EXTERN int camera_open(char *devname);
EXTERN int camera_close(int camfd);
EXTERN int camera_reset(int camfd, char *timfile, char *utilfile);
EXTERN int camera_quickreset(int camfd);
EXTERN int camera_download(int camfd, char *timfile, char *utilfile);
EXTERN int camera_expose(int camfd, char *tempfilename, camreq *req, double *t_expose1);
EXTERN int camera_initialize(int camfd, struct camreq *rq, double *t_expose1,
	   int clear_flag);
EXTERN int camera_wait(int camfd,  struct camreq *rq);
/*
EXTERN int camera_readout(int camfd, char *rawfilename, struct camreq *rq);
*/
EXTERN int camera_readout(int camfd,  struct camreq *rq);
EXTERN int camera_descramble(char *rawfilename, struct camreq *rq);



/*
 * Camera internal commands
 * These routines provide the functionality to implement the above
 * commands by manipulating the various components of the camera,
 * including loading code into the controller, read/write registers, etc.
 */
EXTERN int camdsp_aex(int camfd, int boardid);
EXTERN int camdsp_clr(int camfd, int boardid);
EXTERN int camdsp_csh(int camfd, int boardid);
EXTERN int camdsp_hgn(int camfd, int boardid);
EXTERN int camdsp_idl(int camfd, int boardid);
EXTERN int camdsp_lda(int camfd, int boardid, int data);
EXTERN int camdsp_lgn(int camfd, int boardid);
EXTERN int camdsp_osh(int camfd, int boardid);
EXTERN int camdsp_pex(int camfd, int boardid);
EXTERN int camdsp_pix(int camfd, int boardid);
EXTERN int camdsp_pon(int camfd, int boardid);
EXTERN int camdsp_rad(int camfd, int boardid);
EXTERN int camdsp_rdc(int camfd, int boardid);
EXTERN int camdsp_rdm(int camfd, int boardid, int memspace, int address);
EXTERN int camdsp_rex(int camfd, int boardid);
EXTERN int camdsp_pskip(int camfd, int boardid);
EXTERN int camdsp_rst(int camfd, int boardid);
EXTERN int camdsp_sbv(int camfd, int boardid);
EXTERN int camdsp_sex(int camfd, int boardid);
EXTERN int camdsp_stp(int camfd, int boardid);
EXTERN int camdsp_syr(int camfd, int boardid);
EXTERN int camdsp_tad(int camfd, int boardid);
EXTERN int camdsp_tck(int camfd, int boardid);
EXTERN int camdsp_tda(int camfd, int boardid);
EXTERN int camdsp_tdc(int camfd, int boardid);
EXTERN int camdsp_tdg(int camfd, int boardid);
EXTERN int camdsp_tdl(int camfd, int boardid, int data);
EXTERN int camdsp_wrm(int camfd, int boardid, int memspace, int address,
			int data);

/* load code to camera DSP's */
EXTERN int camdsp_download(int camfd, int board, char *filename);

/*
 * Camera device driver interface routines
 * This is the least-preferred, lowest-level interface to the camera.
 */
EXTERN int camif_open(char *devname);
EXTERN int camif_close(int camfd);
EXTERN int camif_reset(int camfd);
EXTERN int camif_settimeout(int camfd, clock_t *usecs_p);
EXTERN int camif_gettimeout(int camfd, clock_t *usecs_p);
EXTERN int camif_setblocklen(int camfd, int block_length);
EXTERN int camif_tx(int camfd, u_char *buf, int nbytes);
EXTERN int camif_rx(int camfd, u_char *buf, int nbytes);
EXTERN int camif_ww(int camfd, int width);
EXTERN int camif_cmd(int camfd, u_int cmd, u_char *arg);

#endif /* _ATROCAM_H_ */
