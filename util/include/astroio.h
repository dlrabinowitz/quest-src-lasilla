#ifndef _ASTROIO_H_
#define _ASTROIO_H_
 #define NEAT   /*for compatibility with NEAT system on Dilbert*/
/* astroio.h 
 * @(#)astroio.h	1.5 08 Nov 1995
 *
 * IOCTL definitions for ASTRO driver and applications which use it
 *
 * Steve Groom, JPL
 * 7/6/95
 */

#define ASTRO_DMA_BUFLEN        (63*1024)       /* maxmimum DMA buffer size */

/*
 * IOCTL definitions
 */
#ifdef NEAT
/* For NEAT: */

#define ASTROIO_WRTDMACSR       0x80040000	/* write DMA CSR */
#define ASTROIO_WRTDMAAR        0x80040001	/* write DMA address reg */
#define ASTROIO_WRTDMABCR       0x80040002	/* write DMA byte count reg */
#define ASTROIO_WRTDMAUNU       0x80000003	/* write DMA "unused" reg */
#define ASTROIO_WRTDMAWW        0x80040004	/* write DMA word width reg */
#define ASTROIO_WRTDCHAN        0x80000005	/* write DMA D chan reg */
#define ASTROIO_WRTECHAN        0x80020006	/* write DMA E chan reg */
#define ASTROIO_RESET        	0x80000007	/* reset board */
#define ASTROIO_SETTIMEOUT     	0x80040008	/* set DMA timeout */
#define ASTROIO_WRTDMABL        0x80000009	/* write DMA block length for 
						   handshaking */
#define ASTROIO_RDDMACSR        0x40040000	/* read DMA CSR */
#define ASTROIO_RDDMAAR         0x40040001	/* read DMA address reg */
#define ASTROIO_RDDMABCR        0x40040002	/* read DMA byte count reg */
#define ASTROIO_GETTIMEOUT     	0x40040003	/* get DMA timeout */

#else
/* For STEPS */

#define ASTROIO_WRTDMACSR       0x80040000	/* write DMA CSR */
#define ASTROIO_WRTDMAAR        0x80040001	/* write DMA address reg */
#define ASTROIO_WRTDMABCR       0x80040002	/* write DMA byte count reg */
#define ASTROIO_WRTDMAUNU       0x80000003	/* write DMA "unused" reg */
#define ASTROIO_WRTDMAWW        0x80040004	/* write DMA word width reg */
#define ASTROIO_WRTDCHAN        0x80000005	/* write DMA D chan reg */
#define ASTROIO_WRTECHAN        0x80020006	/* write DMA E chan reg */
#define ASTROIO_WRTDMABL        0x80000007	/* write DMA block length for 
						   handshaking */
#define ASTROIO_SETTIMEOUT     	0x80040008	/* set DMA timeout */
#define ASTROIO_RESET        	0x80000009	/* reset board */

#define ASTROIO_RDDMACSR        0x40040000	/* read DMA CSR */
#define ASTROIO_RDDMAAR         0x40040001	/* read DMA address reg */
#define ASTROIO_RDDMABCR        0x40040002	/* read DMA byte count reg */
#define ASTROIO_GETTIMEOUT     	0x40040003	/* get DMA timeout */

#endif

/* encode size in WRTDMAUNU, WRTDCHAN  commands */
#define ASTROIO(cmd,cnt) \
	(cmd | ((cnt&0x000000ff)<<16))		/* encode count in cmd */

#endif
