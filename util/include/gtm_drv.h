#ifndef _GTMDRV_H_
#define _GTMDRV_H_
/* gtm_drv.h
 * @(#)gtm_drv.h	1.5 25 Oct 1995
 *
 * GEODSS telescope mount driver definitions
 *
 * Steve Groom, JPL
 * 5/1/95
 */

#include <idrio.h>	/* IKON DR11-W definitions */

/*** GTM function call prototypes ***/
#ifdef __cplusplus
#define EXTERN	extern "C"
#else
#define EXTERN	extern
#endif

/* function prototypes */
EXTERN int gtm_open();
EXTERN int gtm_io(int fd, u_int cmd, u_int *data);
EXTERN int gtm_dumpregs(int fd);
EXTERN int gtm_close(int fd);
EXTERN int gtm_wait_ssi(int fd);

/*** GTM definitions ***/
#define GTM_DEVICE	"/dev/idr0"

/******* DR11-C device emulation ******/
/* output handshake bits, in latched functions register */
#define GTM_DATA_RDY	IDR_FCN1	/* Data Ready */
#define GTM_DATA_XMTD	IDR_FCN2	/* Data Transmitted */
#define GTM_DATA_SEL	IDR_FCN3	/* Data/Address Select, aka CSR1 */
#define GTM_DATA_MASK	(GTM_DATA_RDY | GTM_DATA_XMTD | GTM_DATA_SEL)

/* input handshake bits, in status register */
#define GTM_REQ_A	IDR_STTA	/* Request A */
#define GTM_REQ_B	IDR_STTB	/* Request B */
#define GTM_REQ_MASK	(GTM_REQ_A | GTM_REQ_B)

/* input interrupt word bits */
#define GTM_INTR_SYS	0x01	/* system interrupt from T/M */
#define GTM_INTR_SSI	0x02	/* servo settled interrupt */
#define GTM_INTR_TPE	0x04	/* tower-side BIU parity error */
#define GTM_INTR_DPE	0x08	/* DPG-side BIU parity error */ /* ?? */
#define GTM_INTR_BIU	0x10	/* BIU/MPACS interface error */ /* ?? */
#define GTM_INTR_MASK	0x1f	/* mask of known errors */

/******* Telescope command words ******/
/* beware, these are all octal!  The leading zero is required! */
/* data readout commands */
#define GTM_CMD_RDMD_POL	0005	/* read mode polar axis */
#define GTM_CMD_RDMD_GDEC	0006	/* read mode gdec axis */
#define GTM_CMD_RDPE_POL_C	0011	/* read pos. encoder, polar coarse */
#define GTM_CMD_RDPE_GDEC_C	0012	/* read pos. encoder, gdec coarse */
#define GTM_CMD_RDPE_POL_F	0015	/* read pos. encoder, polar fine */
#define GTM_CMD_RDPE_GDEC_F	0016	/* read pos. encoder, gdec fine */
#define GTM_CMD_RDST_OPTICS	0023	/* read optics status reg */
#define GTM_CMD_RDST_ENC_POL	0041	/* read encoder status reg, polar */
#define GTM_CMD_RDST_ENC_GDEC	0042	/* read encoder status reg, gdec */
#define GTM_CMD_RDST_SRV_POL	0045	/* read servo status reg, polar */
#define GTM_CMD_RDST_SRV_GDEC	0046	/* read servo status reg, gdec */
/* mount set commands */
#define GTM_CMD_SETMD_POL	0101	/* set mode polar axis */
#define GTM_CMD_SETMD_GDEC	0102	/* set mode gdec axis */
#define GTM_CMD_SETPOS_DOME	0107	/* set dome position */
#define GTM_CMD_SETPOS_POL_C	0111	/* set pos. polar coarse */
#define GTM_CMD_SETPOS_GDEC_C	0112	/* set pos. gdec coarse */
#define GTM_CMD_SETPOS_FOCUS	0113	/* set focus position */
#define GTM_CMD_SETPOS_POL_F	0115	/* set pos. polar fine */
#define GTM_CMD_SETPOS_GDEC_F	0116	/* set pos. gdec fine */
#define GTM_CMD_SETRT_POL_C	0121	/* set rate polar coarse */
#define GTM_CMD_SETRT_GDEC_C	0122	/* set rate gdec coarse */
#define GTM_CMD_SETPOS_BEAM	0123	/* set beamsplitter position */
#define GTM_CMD_SETRT_POL_F	0125	/* set rate polar fine */
#define GTM_CMD_SETRT_GDEC_F	0126	/* set rate gdec fine */
/* keyboard display control */
#define GTM_CMD_SETKEY_C	0300	/* keyboard display coarse data */
#define GTM_CMD_SETKEY_F	0304	/* keyboard display coarse data */
#define GTM_CMD_SETKEY_POL	0305	/* keyboard display mode polar */
#define GTM_CMD_SETKEY_GDEC	0306	/* keyboard display mode gdec */
/* interrupt control */
#define GTM_CMD_RDINTR		0003	/* interrupt read */
#define GTM_CMD_SETINTR_ENB	0302	/* interrupt enable/disable */
#define GTM_CMD_SETINTR_RST	0303	/* interrupt reset */

/******** Telescope command data values ********/
/* pol/gdec mode values for RDMD/SETMD */
#define GTM_MD_OFF		0x00	/* off */
#define GTM_MD_POS		0x01	/* position mode */
#define GTM_MD_RATE		0x02	/* rate mode */


/******** Telescope mount I/O timeouts ********/
#define GTM_TIMEOUT_READ	2	/* seconds */
#define GTM_TIMEOUT_SSI		30	/* seconds */


/******** Telescope mount status register values *******/
/** Encoder Status Readout registers **/
#define GTM_ESR_LOW_INHIBIT	0x80	/* low-res inhibit */
#define GTM_ESR_LOW_PHASELOCK	0x40	/* low-res phase lock */
#define GTM_ESR_LOW_AXISMSB	0x20	/* low-res axis MSB */
#define GTM_ESR_LOW_AXISLSB	0x10	/* low-res axis LSB */
#define GTM_ESR_HIGH_INHIBIT	0x08	/* high-res inhibit */
#define GTM_ESR_HIGH_PHASELOCK	0x04	/* high-res phase lock */
#define GTM_ESR_HIGH_AXISMSB	0x02	/* high-res axis MSB */
#define GTM_ESR_HIGH_AXISLSB	0x01	/* high-res axis LSB */

/** Servo Status Readout registers **/
#define GTM_SSR_SETTLE		0x0100	/* servo settle */
#define GTM_SSR_ESTOP		0x0080	/* emergency stop activated */
#define GTM_SSR_RATE1		0x0040	/* rate trip 1 */
#define GTM_SSR_RATE2		0x0020	/* rate trip 2 */
#define GTM_SSR_TILT		0x0010	/* tilt */
#define GTM_SSR_BRAKE		0x0008	/* brake set */
#define GTM_SSR_STOW		0x0004	/* stow locks set */
#define GTM_SSR_LIM2		0x0002	/* lim2 */
#define GTM_SSR_LIM1		0x0001	/* lim1 */

#endif /* _GTMDRV_H_ */
