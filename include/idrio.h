/* IKON Corporation 2617 Western Ave Seattle, WA 98121   (206) 728-6465 */

/* 16 September 1991	BETA version	*/

/* 3 January 1992	modified for 24 bit range count and multicycle
			error flag in FPGA3.  also made sleeps breakable.
*/


/************************************************************************
*									*
*	This driver is provided at no charge to IKON's customers 	*
*	in the hope that it will assist them in understanding and	*
*	using IKON's Sbus DR11 emulator - model 10103.  This code	*
*	is intended to be a working and (relatively!) bug free driver	*
*	when running on the machine and OS rev available to IKON.	*
*	IKON will attempt to keep this code running on current OS and	*
*	hardware from SUN - and others - but does not guarantee this.	*
*	The user is encouraged to contact IKON with comments,		*
*	suggestions, and BUG REPORTS.					*
************************************************************************/




/* definition file for users of IKON driver for Sbus DR11 emulator - 10103  */
/* also used inside driver code !!! don't mess with this unless you mean it!*/


/*

The 10103 uses the LSI logic L64853A Sbus DMA controller.  The D channel
is used for all programmed i/o and DMA transfers.
The E channel us unused.  The register mappings are shown below.
The base addresses of the two sets
of registers - DMAIO and IDR - are determined by the address bits
used by the L64853 to select the register spaces.
In the current implementation, bits A15 &
A14 are used.  This is reported by the Fcode prom, and will be transparent to
the driver code.  In order to reduce the loading on each Sbus address line, the
IDR registers are placed on 16 byte boundaries, and occupy 128 bytes of space.


The DMAIO registers are 32 bits wide, the DR11 registers are 8 bits wide.


                               A15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0

DMAIO CSR                        0  1  X  X  X  X  X  X  X  X  X  X  0  0  0  X 

DMAIO ADD                        0  1  X  X  X  X  X  X  X  X  X  X  0  1  0  X

DMAIO RANGE                      0  1  X  X  X  X  X  X  X  X  X  X  1  0  0  X 


DR11 MODE REGISTER               1  0  X  X  X  X  X  X  X  0  0  0  X  X  X  X 

DR11 LATCHED FUNCTIONS           1  0  X  X  X  X  X  X  X  0  0  1  X  X  X  X 

DR11 PULSE CMD - FLAGS/STATUS    1  0  X  X  X  X  X  X  X  0  1  0  X  X  X  X 

DR11 DATA HIGH                   1  0  X  X  X  X  X  X  X  0  1  1  X  X  X  X 

DR11 DATA LOW                    1  0  X  X  X  X  X  X  X  1  0  0  X  X  X  X 

DR11 RANGE HIGH (4 bits FPGA2)   1  0  X  X  X  X  X  X  X  1  0  1  X  X  X  X

DR11 RANGE MID                   1  0  X  X  X  X  X  X  X  1  1  0  X  X  X  X

DR11 RANGE LOW                   1  0  X  X  X  X  X  X  X  1  1  1  X  X  X  X

*/







/* ------------ DMAIO CSR  Register bit assignments

31-28 	R	Device Id	1001	(= REV A - required!)
27	R	Next Address Loaded
26	R	Address Loaded
25	R	DMA on
24	RW	Enable Next Address autoload
23	RW	Disable TC interrupts
22	RW	Enbale "faster" DMA handshake
21	R	Lance error (E channel - not used)
20	RW	ALE/-AS polarity select (not used)
19-16	R	reserved - not used
15	RW	ILAC.  Write this bit to 0
14	R	TC
13	RW	Enable Byte Counter = 1
12-10	R	reserved - not used
9	RW	D Channel DMA Enable 1= Respond to D Chan requests
8	RW	D Channel Write Operation  1 = Write memory   0 = Read memory
7	RW	Reset   1 = Reset state
6	RW	Slave Size error
5	W	Flush Buffer. 1 = Reset DRAINING, TC, ERR PENDING
4	RW	Interrupt Enable.  1 = Pass interrupt requests to SBus INTREQ
		Interrupt sources are D_IRQ*, Error Pending and TC reached.
3-2	R	DRAINING = 11 if cache being drained to memory.
1	R	Error Pending.  Set if an error during a D Channel operation.
		Parity, protection or timeout.
0	R	Interrupt Pending (1 = Asserted.).  Latched OR of D_IRQ* or TC

----*/


/*--------------DR11 Mode register bits

7	RDYT	RW	READY ends at BUSY TE if on, during BUSY if off
6	FMOD	RW	use full input fifo if on, 1/2 fifo & faster if off
5	BDIS	RW	disable CYCLE RQ b if on
4	SWAP	RW	swap DMA bytes if on
3	CRQP	RW	CYCLE RQx rising edge active if on, falling edge if off
2	BSYP	RW	BUSY active high if on, active low if off
1	SPD1	RW	DR11 handshake speed selection, fastest = 00
0	SPD0	RW

	MODE register reset by bus init only - not by master clear (MCLR)
----*/


/*--------------DR11 Latched Functions register bits

7	RW	EORM	End-of-range interrupt mask
6	RW	ATTM	ATTENTION interrupt mask
5	RW	RDIS	disable DR11 range counter
4	RW	DMON	enable DR11 control array
3	RW	FCN3	DR11 function bits to device
2	RW	FCN2
1	RW	FCN1
0	RW	DMIN	master DR11 direction control (DMA & P-I/O) 1=input
----*/


/*--------------DR11 Pulse Commands (write only)

7	W	REOR	reset End-of-range flag
6	W	RATN	reset ATTENTION flag
5	W	TERM	set READY true & reset DR11 handshake logic
4	W	MCLR	master clear DR11 registers & logic
3	W	INIT	send INIT pulse to device
2	W	ACF2	send ACLO FNCT2 pulse to device
1	W	CYCL	DR11 cycle request
0	W	GO	send GO to device & enable DR11 handshake logic

----*/


/*--------------DR11 Status/Flags (read only)

7	R	EORF	DR11 range counter end-of-range
6	R	ATTF	ATTENTION flag (sets on ATTENTION rising edge)
5	R	ATTN	follows ATTENTION signal
4	R	MCER	multicycle error flag (FPGA3 and later)	
3	R	STTA	status bits from device
2	R	STTB
1	R	STTC
0	R	REDY	DR11 READY (0 = ready for cycle requests)

----*/


/*--------------DR11 Data High

	high 8 bits of 16 bit word to/from device

----*/


/*--------------DR11 Data Low

	low 8 bits of 16 bit word to/from device

----*/


/*--------------DR11 Range High

	high 8 bits of 24 bit DR11 range counter
	range 20 bits only in FPGA2, 24 bits for FPGA3 and later

----*/


/*--------------DR11 Range Mid

	middle 8 bits of 24 bit DR11 range counter

----*/


/*--------------DR11 Range Low

	low 8 bits of 24 bit DR11 range counter

----*/



#ifndef	_IOCTL_
#include <sys/ioctl.h>
#endif



/* define DMAIO register bits */

#define	DMAIO_EN_NEXT		0X01000000 /* 1 = next add autoload enabled */
#define	DMAIO_TCI_DIS		0X00800000 /* 1 = disable TC interrupts */
#define DMAIO_FASTER		0X00400000 /* 1 = fast D channel DMA xfers */
#define DMAIO_COUNT_ENABLE	0X00002000 /* 1 = enb Byte Count, 0 = disable */
#define DMAIO_DMA_ENABLE	0X00000200 /* 1 = enable DMA operation  */
#define DMAIO_DMA_MEM_WRITE	0X00000100 /* 1 = Write mem 0 = Read mem */
#define DMAIO_DEV_RESET 	0X00000080 /* 1 = enable, 0 = disable */
#define DMAIO_FLUSH_BFR		0X00000020 /* 1 = Reset DRAINING, ERR and TC */
#define DMAIO_INT_ENABLE	0X00000010 /* 1 = enable, 0 = disable */

#define DMAIO_DEV_ID		0XF0000000 /* dev ID field = 1001 for REV A */
#define	DMAIO_NA_LDD		0X08000000 /* 1 = next address loaded */
#define	DMAIO_A_LDD		0X04000000 /* 1 = address loaded */
#define DMAIO_DVMA_ON		0X02000000 /* 1 = DVMA transfers enabled */
#define DMAIO_TC         	0X00004000 /* 1 = TC Reached */
#define DMAIO_SLAVE_ERR		0X00000040 /* 1 = slave err during DVMA */
#define DMAIO_DRAINING   	0X0000000c /* 11 = draining to memory */
#define DMAIO_ERR_PENDING	0X00000002 /* 1 = Active, clears on read */
#define DMAIO_INT_PENDING	0X00000001 /* 1 = Active, clears on read */



/* define DR11 register bits */

/* mode register	*/

#define	IDR_RDYT	0x80	/* select delayed ready timing		*/
#define IDR_FMOD	0x40	/* select slow input fifo & use all of it*/
#define IDR_BDIS	0x20	/* disable CYCLE RQ B			*/
#define IDR_SWAP	0x10	/* swap DR11 dma bytes			*/
#define	IDR_CRQP	0x08	/* select CYCLE RQ falling edge active	*/
#define IDR_BSYP	0x04	/* select BUSY active high		*/
#define IDR_SPEED_MASK	0x03	/* bottom two bits select speed		*/
#define	IDR_SPD_0	0x00	/* fastest DR11 timing			*/
#define	IDR_SPD_1	0x01
#define IDR_SPD_2	0x02
#define	IDR_SPD_3	0x03	/* slowest DR11 timing			*/


/* latched functions	*/

#define	IDR_EORM	0x80	/* DR11 end-of-range interrupt mask	*/
#define	IDR_ATTM	0x40	/* ATTENTION interrupt mask		*/
#define	IDR_RDIS	0x20	/* disable DR11 range counter		*/
#define		IDR_RDISX  0x2000	/* use this shifted bit for the	*/
					/* SET_MODE ioctl call		*/
#define	IDR_DMON	0x10	/* enable DR11 dma transfers		*/
#define	IDR_FCN3	0x08	/* function bits			*/
#define	IDR_FCN2	0x04
#define	IDR_FCN1	0x02
#define		IDR_FMASK (IDR_FCN3 | IDR_FCN2 | IDR_FCN1) /* f mask	*/
#define	IDR_DMIN	0x01	/* master direction control 1 = input	*/


/* pulse commands	*/

#define	IDR_REOR	0x80	/* reset end-of-range flag		*/
#define	IDR_RATN	0x40	/* reset attention flag			*/
#define	IDR_TERM	0x20	/* set READY true & clr handshake logic	*/
#define	IDR_MCLR	0x10	/* reset DR11 control logic & fifos	*/
#define	IDR_INIT	0x08	/* send init to device			*/
#define	IDR_ACF2	0x04	/* send acf2 pulse to device		*/
#define	IDR_CYCL	0x02	/* force cycle request			*/
#define	IDR_GO		0x01	/* send GO & enable DR11 handshake stuff*/


/* flags/status		*/

#define	IDR_EORF	0x80	/* DR11 end-of-range flag		*/
#define	IDR_ATTF	0x40	/* attention flag			*/
#define	IDR_ATTN	0x20	/* attentin flag			*/
#define	IDR_MCER	0x10	/* multicycle error flag		*/
#define	IDR_STAT_MASK	0x0E	/* mask status bits			*/
#define	IDR_STTA	0x08
#define	IDR_STTB	0x04
#define	IDR_STTC	0x02
#define	IDR_REDY	0x01	/* master DR11 ready bit 0=dma enabled	*/





/* the ioctl() function call looks like:

		 ioctl(filedescriptor,command,argument)

argument is used in some of the idr ioctl calls to provide values to
the driver ioctl routine, or return values to the calling program.
the argument is restricted to a maximum of 255
bytes - by unix, and by the driver.

the following ioctl commands
are available to programs using the idr driver:

IDRIO_SET_MODE		sets the mode register bits and the RDIS bit in
			the latched functions register
IDRIO_IMM_FCN		sets the function bits immediately
IRDIO_READ_FCN		sets the function bits before each block read
IDRIO_WRITE_FCN		sets the function bits before each block write
IDRIO_IMM_PULSE		issues pulse commands immediately
IDRIO_READ_PULSE	issues pulse commands at each block read
IDRIO_WRITE_PULSE	issues pulse commands at each block write
IDRIO_SET_DMA_TIME	sets dma timeout value
IDRIO_SET_ATTN_TIME	sets wait for attention timeout value
IDRIO_SET_RDY_TIME	sets wait for ready timeout value
IDRIO_ATTN_WAIT		waits for attention 0-to-1 transition
IDRIO_RDY_WAIT		waits for READY
IDRIO_GET_STATUS	gets flags/status register
IDRIO_GET_RANGE		gets range counter value
IDRIO_GET_REGS		gets all registers including DMA chip's
			note that latch reg bit DMIN is forced to 1 by
			this ioctl(to allow reading data in reg)
IDRIO_GET_FLAGS		returns various driver flag and error bits
IDRIO_DATA_OUT		writes 16 bits to DR11 output latches
IDRIO_DATA_IN		reads 16 bits from DR11 input latches
IDRIO_SET_RANGE		sets DR11 range counter in manual mode
IDRIO_AUTO		selects auto mode (default). auto mode causes DR11
			range & controls to be set up for each read/write
			call at same time as DVMA logic is set up. DVMA and
			DR11 blocks are set to the same size, and a single
			DVMA block is used per DR11 block.  in auto mode, all
			that is necessary to do dma transfers is read or write
			calls.
IDRIO_MANUAL		selects manual mode.  manual mode causes read/write
			calls to set up DVMA controller only.  DR11 range
			and controls must be set up explicitly by calling pgm.
			manual mode allows multiple read/write calls per DR11
			block, or disabling the DR11 range counter and doing
			continuous transfers.
IDRIO_START_READ
IDRIO_START_WRITE	in manual mode, enables overall block-issued before
			DR11 logic is enabled with GO command. and before the
			first unix read or write call.
IDRIO_BLOCK_END		in manual mode, disables overall block transfer-issued
			after all DR11 transfers are complete, typically after
			a wait for ready has returned successfully.

*/



/* the ioctl command codes conform to the unix pattern:

	the top 3 bits of the 32 bit value indicate whether arguments
	are to be copied in, copied out, both, or neither.

	0x80000000 = copy in
	0x40000000 = copy out
	0x20000000 = no argument transfer
	0xC0000000 = copy in and out

	the number of bytes in the argument is encoded in the lower
	8 bits of the upper half of the u_int, and the actual command
	is encoded in the lower half.  a rather arbitrary character,
	which is intended to identify the driver, is also encoded in
	the lower half of the command.  it becomes part of the command
	value.

	for all commands the arg length is part of
	the command value.

	commands which require arguments - in or out - will pass those
	arguments as unsigned integers.

	the magic character that identifies this driver is hereby
	(arbitrarily) chosen to be 'D'.

	the following ioctl commands are defined using pre-existing
	ioctl command macros.  the CMD_MASK and COUNT_MASK values
	defined MUST match the usage in ioccom.h. refer to that include
	file for further information.
*/


#define IDRIO_CMD_MASK 0xE000FFFF	/* cmd in top 3 and bottom 16 bits */
#define IDRIO_COUNT_MASK 0x00FF0000	/* arg byte count here		   */

#ifdef __STDC__

#define IDRIO_SET_MODE		_IOW('D',1,int)	/* set mode reg & RDIS 	*/

#define	IDRIO_IMM_FCN		_IOW('D',2,int)	/* set function bits NOW*/

#define IDRIO_READ_FCN		_IOW('D',3,int)	/* set fcn bits at read start*/

#define	IDRIO_WRITE_FCN		_IOW('D',4,int)	/* set fcn bits at write time*/	

#define	IDRIO_IMM_PULSE		_IOW('D',5,int)	/* issue pulses NOW	*/

#define	IDRIO_READ_PULSE	_IOW('D',6,int)	/* issue pulses at read start*/

#define	IDRIO_WRITE_PULSE	_IOW('D',7,int)	/* issue pulses at write time*/

#define	IDRIO_SET_DMA_TIME	_IOW('D',8,int)	/* DVMA block timeout in secs*/
						/* also controls EOR timeout */
						/* in auto mode. manual eor  */
						/* wait uses RDY_TIME        */

#define	IDRIO_SET_ATTN_TIME	_IOW('D',9,int)	/* ATTENTION wait timeout #  */

#define	IDRIO_SET_RDY_TIME	_IOW('D',10,int)	/* READY wait timeout in secs*/

#define	IDRIO_ATTN_WAIT		_IO('D',11)	/* wait for ATTENTION flag   */

#define	IDRIO_RDY_WAIT		_IO('D',12)	/* wait for DR11 READY	*/

#define	IDRIO_GET_STATUS	_IOR('D',13,int)	/* returns DR11 status reg */

#define	IDRIO_GET_RANGE		_IOR('D',14,int)	/* returns DR11 range reg  */
						/* gets actual bits which  */
						/* are initially set to the*/
						/* WORD count minus 1 !!!  */
						/* and decrement per DR11  */
						/* transfer (16 bit words) */

#define IDRIO_GET_REGS	_IORN('D',15,44)		/* puts all regs in arg */

	/* returns all board registers.
	   DMAIO registers are all 32 bits wide. DR11 registers are 8
	   bits wide. each is returned in a 32 bit longword in the
	   following order:

	(u_long larg[11])

	larg[0]  =	DMAIO CSR 
	larg[1]  =	DMAIO ADD
	larg[2]  =	DMAIO RANGE
	larg[3]  =	DR11 MODE
	larg[4]  =	DR11 LATCHED FUNCTIONS
	larg[5]  =	DR11 FLAGS/STATUS
	larg[6]  =	DR11 DATA HIGH
	larg[7]  =	DR11 DATA LOW
	larg[8]  =	DR11 RANGE HIGH (4 bits pre FPGA3)
	larg[9]  =	DR11 RANGE MID
	larg[10] =	DR11 RANGE LOW
	
	*/

#define	IDRIO_GET_FLAGS		_IOR('D',16,int)	/* returns driver unit_flags */

#define		IDR_DVMA_WAIT	0x80000000	/* waiting for t/c	*/
#define		IDR_EOR_WAIT	0x40000000	/* waiting for EOR	*/
#define		IDR_ATTN_WAIT	0x20000000	/* waiting for attention*/
#define		IDR_RDY_WAIT	0x10000000	/* waiting for ready	*/
#define		IDR_DVMA_TIMEOUT 0x08000000	/* dma wait timed out	*/
#define		IDR_EOR_TIMEOUT	0x04000000	/* eor wait timeout	*/
#define		IDR_ATTN_TIMEOUT 0x02000000	/* attn timeout	*/
#define		IDR_RDY_TIMEOUT	0x01000000	/* ready wait timeout	*/
#define		IDR_MANUAL	0x00800000	/* manual mode		*/
#define		IDR_INPUT	0x00400000	/* used w/waiting-for	*/
						/* indicates xfer dir	*/
#define		IDR_MCYL_ERR	0x00200000	/* multicycle error	*/
#define		IDR_SIG_RECEIVED 0x00100000	/* sleep term'd by signal*/

#define	IDR_CLEAR_FLAGS ~(IDR_DVMA_WAIT|IDR_EOR_WAIT|IDR_ATTN_WAIT|\
IDR_RDY_WAIT|IDR_DVMA_TIMEOUT|IDR_EOR_TIMEOUT|IDR_ATTN_TIMEOUT|IDR_RDY_TIMEOUT\
|IDR_MCYL_ERR|IDR_SIG_RECEIVED)

/* idr_clear_flags used to reset the above flags (INSIDE DRIVER ONLY!!!!!!) */

#define	IDRIO_DATA_OUT		_IOW('D',17,int)	/* 16 bits to DR11 output reg*/

#define	IDRIO_DATA_IN		_IOR('D',18,int)	/* reads DR11 input reg	*/

#define	IDRIO_SET_RANGE		_IOW('D',19,int)	/* sets DR11 range reg	*/
						/* int is WORD count-1	*/
						/* it is the actual value*/
						/* to be plugged into the */
						/* range register!!!!!!	*/

#define	IDRIO_AUTO		_IO('D',20)	/* selects manual mode	*/
						/* auto is driver default */

#define	IDRIO_MANUAL		_IO('D',21)	/* selects manual mode	*/ 
						/* manual requires direct */
						/* control of DR11 range and */
						/* block start and end	*/

#define	IDRIO_START_READ	_IO('D',22)	/* enables overall block xfer*/
						/* manual mode only!	*/

#define	IDRIO_START_WRITE	_IO('D',23)	/* enables write block xfer  */

#define	IDRIO_BLOCK_END		_IO('D',24)	/* disables block xfer(ends) */
						/* manual mode only	*/

#else /* ! __STDC__ */

#define IDRIO_SET_MODE		_IOW(D,1,int)	/* set mode reg & RDIS 	*/

#define	IDRIO_IMM_FCN		_IOW(D,2,int)	/* set function bits NOW*/

#define IDRIO_READ_FCN		_IOW(D,3,int)	/* set fcn bits at read start*/

#define	IDRIO_WRITE_FCN		_IOW(D,4,int)	/* set fcn bits at write time*/	

#define	IDRIO_IMM_PULSE		_IOW(D,5,int)	/* issue pulses NOW	*/

#define	IDRIO_READ_PULSE	_IOW(D,6,int)	/* issue pulses at read start*/

#define	IDRIO_WRITE_PULSE	_IOW(D,7,int)	/* issue pulses at write time*/

#define	IDRIO_SET_DMA_TIME	_IOW(D,8,int)	/* DVMA block timeout in secs*/
						/* also controls EOR timeout */
						/* in auto mode. manual eor  */
						/* wait uses RDY_TIME        */

#define	IDRIO_SET_ATTN_TIME	_IOW(D,9,int)	/* ATTENTION wait timeout #  */

#define	IDRIO_SET_RDY_TIME	_IOW(D,10,int)	/* READY wait timeout in secs*/

#define	IDRIO_ATTN_WAIT		_IO(D,11)	/* wait for ATTENTION flag   */

#define	IDRIO_RDY_WAIT		_IO(D,12)	/* wait for DR11 READY	*/

#define	IDRIO_GET_STATUS	_IOR(D,13,int)	/* returns DR11 status reg */

#define	IDRIO_GET_RANGE		_IOR(D,14,int)	/* returns DR11 range reg  */
						/* gets actual bits which  */
						/* are initially set to the*/
						/* WORD count minus 1 !!!  */
						/* and decrement per DR11  */
						/* transfer (16 bit words) */

#define IDRIO_GET_REGS	_IORN(D,15,44)		/* puts all regs in arg */

	/* returns all board registers.
	   DMAIO registers are all 32 bits wide. DR11 registers are 8
	   bits wide. each is returned in a 32 bit longword in the
	   following order:

	(u_long larg[11])

	larg[0]  =	DMAIO CSR 
	larg[1]  =	DMAIO ADD
	larg[2]  =	DMAIO RANGE
	larg[3]  =	DR11 MODE
	larg[4]  =	DR11 LATCHED FUNCTIONS
	larg[5]  =	DR11 FLAGS/STATUS
	larg[6]  =	DR11 DATA HIGH
	larg[7]  =	DR11 DATA LOW
	larg[8]  =	DR11 RANGE HIGH (4 bits pre FPGA3)
	larg[9]  =	DR11 RANGE MID
	larg[10] =	DR11 RANGE LOW
	
	*/

#define	IDRIO_GET_FLAGS		_IOR(D,16,int)	/* returns driver unit_flags */

#define		IDR_DVMA_WAIT	0x80000000	/* waiting for t/c	*/
#define		IDR_EOR_WAIT	0x40000000	/* waiting for EOR	*/
#define		IDR_ATTN_WAIT	0x20000000	/* waiting for attention*/
#define		IDR_RDY_WAIT	0x10000000	/* waiting for ready	*/
#define		IDR_DVMA_TIMEOUT 0x08000000	/* dma wait timed out	*/
#define		IDR_EOR_TIMEOUT	0x04000000	/* eor wait timeout	*/
#define		IDR_ATTN_TIMEOUT 0x02000000	/* attn timeout	*/
#define		IDR_RDY_TIMEOUT	0x01000000	/* ready wait timeout	*/
#define		IDR_MANUAL	0x00800000	/* manual mode		*/
#define		IDR_INPUT	0x00400000	/* used w/waiting-for	*/
						/* indicates xfer dir	*/
#define		IDR_MCYL_ERR	0x00200000	/* multicycle error	*/
#define		IDR_SIG_RECEIVED 0x00100000	/* sleep term'd by signal*/

#define	IDR_CLEAR_FLAGS ~(IDR_DVMA_WAIT|IDR_EOR_WAIT|IDR_ATTN_WAIT|\
IDR_RDY_WAIT|IDR_DVMA_TIMEOUT|IDR_EOR_TIMEOUT|IDR_ATTN_TIMEOUT|IDR_RDY_TIMEOUT\
|IDR_MCYL_ERR|IDR_SIG_RECEIVED)

/* idr_clear_flags used to reset the above flags (INSIDE DRIVER ONLY!!!!!!) */

#define	IDRIO_DATA_OUT		_IOW(D,17,int)	/* 16 bits to DR11 output reg*/

#define	IDRIO_DATA_IN		_IOR(D,18,int)	/* reads DR11 input reg	*/

#define	IDRIO_SET_RANGE		_IOW(D,19,int)	/* sets DR11 range reg	*/
						/* int is WORD count-1	*/
						/* it is the actual value*/
						/* to be plugged into the */
						/* range register!!!!!!	*/

#define	IDRIO_AUTO		_IO(D,20)	/* selects manual mode	*/
						/* auto is driver default */

#define	IDRIO_MANUAL		_IO(D,21)	/* selects manual mode	*/ 
						/* manual requires direct */
						/* control of DR11 range and */
						/* block start and end	*/

#define	IDRIO_START_READ	_IO(D,22)	/* enables overall block xfer*/
						/* manual mode only!	*/

#define	IDRIO_START_WRITE	_IO(D,23)	/* enables write block xfer  */

#define	IDRIO_BLOCK_END		_IO(D,24)	/* disables block xfer(ends) */
						/* manual mode only	*/

#endif	/* ! __STDC__ */

/* start read or write enables the DR11 control logic. block end disables it */
/* start read or write should be issued before the first unix read or write  */
/* call in manual mode.  these ioctls do not issue a go or cycle pulse, that */
/* must be done by the calling program AFTER the start ioctl.                */


/* define various flags and constants */

#define	IDRPRI		(PZERO+1)	/* software sleep priority	*/

#define MAX_NIDRS	3		/* max # of devices per driver	*/

#define	IDR_MINPHYS_SIZE	262144	/* NOT USED!!! use system minphys*/

#define	IDR_DR11_MAXBLOCK	0xFFFFFF /* max DR11 range count value	*/
				 	 /* it is WORD count -1		*/
					 /* was 20 bits in FPGA2, is 24 */
					 /* bits wide for FPGA3 & later */

#define DMA_TIME_DEF	30		/* dma time-out default seconds	*/
#define ATTN_TIME_DEF	30		/* fifo empty and <1/2 full time-out*/
#define	RDY_TIME_DEF	30		/* ready wait time default	*/

#define DMA_TIME_MAX	600		/* protect against hanging if 	*/
#define ATTN_TIME_MAX	600		/* if caller asks for giant #	*/
#define	RDY_TIME_MAX	600

#define	DMA_TIME_MIN	10		/* try to prevent timeout during*/
#define ATTN_TIME_MIN	10		/* active dma or legit wait	*/
#define	RDY_TIME_MIN	10

#define MODE_REG_DEF	0		/* sets mode reg at init time	*/
#define	LATCH_REG_DEF	0		/* sets latch reg at init time	*/

#define	READ_FCN_DEF	(IDR_FCN1 | IDR_FCN3)	/* link mode default	*/
#define	READ_PULSE_DEF	IDR_GO		/* go only on read - no aclof2	*/
					/* not quite link mode default	*/
#define	WRITE_FCN_DEF	IDR_FCN3	/* link style out mode		*/
#define	WRITE_PULSE_DEF	(IDR_CYCL | IDR_GO)	/* link style force cycl*/

