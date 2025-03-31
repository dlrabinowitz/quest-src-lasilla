/****************************************************************************
 * focus_drv.h -- include file to help with interfacing the
 * serial port. The serial port is the main interface to the focus
 * which controls the focus settings of the telescope.
 * 
 * Author: Erik Hovland (ehovland@huey.jpl.nasa.gov)
 *
 * Copyright held by the California Institute of Technology, Pasadena,
 *     CA.
 *
 * $RCSfile: focus_drv.h,v $ $Revision: 1.6 $ $Date: 2009/07/23 20:57:09 $
 ***************************************************************************/

#ifndef INC_FOCUS_DRV_H
#define INC_FOCUS_DRV_H

#ifdef __cplusplus
#define EXTERN extern "C"
#else
#define EXTERN extern
#endif

EXTERN int focus_open(void);
EXTERN int focus_io(int fd, int cmd, char *cmd_string);
EXTERN int focus_close(int serial_fd);

/* Focus Serial port definitions */
#define FOCUS_DEVICE "/dev/ttyb"

/*************** Focus telescope command definitions ***************
 * Focus commands are a strange combo of things all ending in a
 * line feed ;). There are 4 things to manipulate in the focus
 * controller. Those are the status, the focus position and the
 * operation of the motor state.
 * Important stuff inbetween ;). There are three basic commands,
 * Status, RA/DEC point, Focus position. There are responses for
 * each and they have specified format. Both RA/DEC and focus are	
 * just acknowledgement messages.
 *******************************************************************/

/* Command headers */
#define FOCUS_CMD_HDR "1"
#define FOCUS_VERSION_CK "ve"
#define FOCUS_POS_MOVE "pa"
#define FOCUS_QUERY_STR "?"
#define FOCUS_MOTOR_PWR_ON "mo"
#define FOCUS_MOTOR_PWR_OFF "mf"

/* Command footer */
#define FOCUS_CMD_FTR "\r\n"

/* Command responses */
#define FOCUS_CTLR_STS_ACK "ESP300 Version 3.00 07/30/99"
#define FOCUS_MP_ON_ACK "1"
#define FOCUS_MP_OFF_ACK "0"

/* Read & Write defines for io function */
#define FOCUS_WRITE 0
#define FOCUS_READ 1

#endif /* INC_FOCUS_DRV_H */
