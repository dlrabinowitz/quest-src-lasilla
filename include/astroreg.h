#ifndef _ASTROREG_H_
#define _ASTROREG_H_
/* astroreg.h
 * @(#)astroreg.h	1.3 25 Oct 1995
 * 
 * Original:
 * Copyright (c)  1990,  L.P. Cygnarowicz, Cygan Systems, (408) 263-9180
 *
 *  Development for SDSU Astronomy Lab by Greg Moore  2/10/95
 *  Ported to Solaris 2.4, Steve Groom, Jet Propulsion Laboratory 6/23/95
 */

/* Definition file for users of the SDSU ASTRO camera SBUS interface board */

/*
 * DMAIO CSR  Register bit assignments
 *
 * 31-28 	R	Device Id		1000
 * 27-16 	R	Reserved		FFF
 * 15		RW	ILAC.  Write this bit to 0.
 * 14		R	TC
 * 13		RW	Enable Byte Counter = 1
 * 12-11	R	D Channel Pack Register Byte Address
 * 10		R	D Channel DMA Cycle Request
 * 9		RW	D Channel DMA Enable 1= Respond to D Chan requests
 * 8		RW	D Channel Write Operation  1=Write mem,  0=Read mem
 * 7		RW	Reset   1 = Reset state
 * 6		RW	Drain Buffer  1 = Write D Chan Pack reg to SBus memory
 *			and clear the Pack Count (Bits 3,2).  This bit clears
 *			automatically.
 * 5		W	Flush Buffer. 1 = Reset Pack Count, Err Pending and TC.
 * 4		RW	Interrupt Enable.  1 = Pass interrupt requests to SBus
 *			INTREQ.  Interrupt sources are D_IRQ*, Error Pending
 *			and TC reached.
 * 3-2		R	D Channel Pack Count.  The number of valid bytes
 *			destined to be written to the SBus.
 * 1		R	Error Pending.  Set on error during a D Channel 
 *			operation; parity, protection or timeout.
 * 0		R	Interrupt Pending (1 = Asserted.).  Latched OR of
 *			D_IRQ* or TC
 */

#define DMAIO_NA_LOADED		0X08000000 /* 1 = next addr loaded */
#define DMAIO_A_LOADED		0X04000000 /* 1 = addr loaded */
#define DMAIO_NEXT_ENABLE	0X01000000 /* 1 = next addr autoload enabled */
#define DMAIO_FASTER		0X00400000 /* 1 = Faster, 0 = Not as fast */
#define DMAIO_COUNT_ENABLE	0X00002000 /* 1 = enable Byte Counter */
#define DMAIO_DMA_ENABLE	0X00000200 /* 1 = enable DMA operation  */
#define DMAIO_DMA_MEM_WRITE	0X00000100 /* 1 = Write mem, 0 = Read mem */
#define DMAIO_DEV_RESET 	0X00000080 /* 1 = enable, 0 = disable */
#define DMAIO_DRAIN_BFR		0X00000040 /* 1 = Write back and clear, bit resets itself */
#define DMAIO_FLUSH_BFR		0X00000020 /* 1 = Reset Pack Count, Err Pending and TC bits */
#define DMAIO_INT_ENABLE	0X00000010 /* 1 = enable, 0 = disable */

#define DMAIO_TC         	0X00004000 /* 1 = TC Reached */
#define DMAIO_PACK_ADR     	0X00001800 /* 1 = marks the two bit address */
#define DMAIO_REQ_PEND		0X00000400 /* 1 = Active DMA request */
#define DMAIO_DRAINING   	0X0000000c /* 11 = draining to memory */
#define DMAIO_ERR_PENDING	0X00000002 /* 1 = Active, clears on read */
#define DMAIO_INT_PENDING	0X00000001 /* 1 = Active, clears on read */

#endif /* _ASTROREG_H_ */
