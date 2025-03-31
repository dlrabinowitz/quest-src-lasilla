/*
 * $RCSfile: dms.c,v $
 * $Revision: 1.7 $
 * $Date: 2010/10/12 14:37:27 $
 *
 * Simple routines to convert degrees to deg/min/sec and vice versa
 * works for hours->h/m/s as well.
 *
 *****************************************************************************
 * Copyright (c) 1995, California Institute of Technology.
 * U.S. Government sponsorship is acknowledged.
 *****************************************************************************
 *
 * Steve Groom, JPL
 * 7/15/95
 *
 * The case where the value of the declination was between -1 and 0
 * degrees yielded positive values for dd:mm:ss. This is corrected to
 * give negative values for mm (if non zero) and/or ss. D. Rabinowitz 97/10/25
 *
 */

#include <stdio.h>
#include <string.h>

/* convert degrees (or hours) to degress/minutes/seconds (or hours/min/sec)
 *
 * SVR4 has no aint() routine, so I did some simpleminded compensation for
 * roundoff error that occurs in converting numbers from double to integer.
 */
void
degdms(double d, int *di, int *mi, double *s)
{
    double m;
    double off = .000000001;
    if (d<0) off = -off;

    /* fprintf(stderr,"degdms: deg %f\n",d); */
    *di = (int) (d + off);
    /* fprintf(stderr,"degdms: di %d\n",*di); */
    m = (d - *di) * 60.0;
    if (m<0) m = -m;	/* change sign if degrees was negative */
    /* fprintf(stderr,"degdms: m %f\n",m); */
    *mi = (int) (m + off);
    /* fprintf(stderr,"degdms: mi %d\n",*mi); */
    *s = (m - *mi) * 60.0;
    if (*s<0) *s = 0;	/* if less than zero, treat as zero */
    if ( *s >= 59.9 ) { 
      *s = 59.9;
    }
 
#if 0  
    /* fprintf(stderr,"degdms: s %f\n",*s); */

    /* Handle special case where d>-1.0 and d<0.0 */
    /* D. Rabinowitz 97 Oct 25 */
    if(d<0.0 &&d>-1.0){
	if(*mi!=0)*mi=-(*mi);
	if(*s!=0)*s=-(*s);
    }
#endif
}

/* convert degrees d to a char representation as +DD:MM:SS.S
 * returns it's buf argument
 */
char *
degdmsstr(double d, char *buf)
{
    int di, mi;
    double s;
    degdms(d,&di,&mi,&s);
    sprintf(buf,"%+02d:%02d:%04.1f",di,mi,s);

    return buf;
    
}

void
hrhms(double h, int *hi, int *mi, double *s)
{
    degdms(h,hi,mi,s);
    return;
}

char *
hrhmsstr(double h, char *buf)
{
    return degdmsstr(h,buf);
    return buf;
}

/***********************/

/* convert hh:mm:ss.ss to float. Returns 0 on success, -1 on failure */
int hms_to_h(char *hms,double *h)
{
   int hh,mm;
   float ss;
   char buf[1024];

   if(strlen(hms)>1024){
       fprintf(stderr,"hms_to_h: hms string too long : %s\n",hms);
       return(-1);
   }
   else if (strncmp(hms+2,":",1)!=0||strncmp(hms+5,":",1)!=0){
       fprintf(stderr,"hms_to_h: hms string not formated correctly : %s\n",hms);
       return(-1);
   }

   strcpy(buf,hms);
   strncpy(buf+2," ",1);
   strncpy(buf+5," ",1);

   sscanf(buf,"%d %d %f",&hh,&mm,&ss);

   *h=hh+(mm/60.0)+(ss/3600.0);

   return(0);
}

/***********************/

/* convert sdd:mm:ss.ss to float. Returns 0 on success, -1 on failure */
int dms_to_d(char *dms,double *d)
{
   int dd,mm,sign;
   float ss;
   char buf[1024];

   if(strlen(dms)>1024){
       fprintf(stderr,"dms_to_d: dms string too long : %s\n",dms);
       return(-1);
   }
   else if(strncmp(dms,"+",1)!=0 && strncmp(dms,"-",1)!=0){ /* no sign string */
       fprintf(stderr,"dms_to_s: dms string not formated correctly : %s\n",dms);
       return(-1);
   }
   else { /* first character is "+" or "-" */
      if (strncmp(dms+3,":",1)!=0||strncmp(dms+6,":",1)!=0){
        fprintf(stderr,"dms_to_s: dms string not formated correctly : %s\n",dms);
        return(-1);
      }
      if(strncmp(dms,"+",1)==0){
         sign=1;
      }
      else{
         sign=-1;
      }
      strcpy(buf,dms+1);
      strncpy(buf+2," ",1);
      strncpy(buf+5," ",1);
   }
     
   sscanf(buf,"%d %d %f",&dd,&mm,&ss);

   *d=dd+(mm/60.0)+(ss/3600.0);

   if(sign<1)*d=-*d;

   return(0);
}

/***********************/

/* convert hhmmss.ss to float. Returns 0 on success, -1 on failure */
int hms_to_h_nocolon(char *hms,double *h)
{
   int hh,mm;
   float ss;
   char buf[1024];

   if(strlen(hms)>1024){
       fprintf(stderr,"hms_to_h_nocolon: hms string too long : %s\n",hms);
       return(-1);
   }
   else if (strncmp(hms+6,".",1)!=0){
        fprintf(stderr,"hms_to_h_nocolon: hms string not formated correctly : %s\n",hms);
        return(-1);
   }

   strcpy(buf,hms);
   strncpy(buf+2," ",1);
   sscanf(buf,"%d",&hh);

   strcpy(buf,hms+2);
   strncpy(buf+2," ",1);
   sscanf(buf,"%d",&mm);

   strcpy(buf,hms+4);
   sscanf(buf,"%f",&ss);

   *h=hh+(mm/60.0)+(ss/3600.0);

   //fprintf(stderr,"hms_to_h_nocolon hh : %d %d %f %f\n",hh,mm,ss, *h);

   return(0);
}

/***********************/

/* convert sddmmss.ss to float. Returns 0 on success, -1 on failure */
int dms_to_d_nocolon(char *dms,double *d)
{
   int dd,mm,sign;
   float ss;
   char buf[1024];

   if(strlen(dms)>1024){
       fprintf(stderr,"dms_to_d_nocolon: dms string too long : %s\n",dms);
       return(-1);
   }
   else if(strncmp(dms,"+",1)!=0 && strncmp(dms,"-",1)!=0){ /* no sign string */
      fprintf(stderr,"dms_to_s_nocolon: dms string not formated correctly : %s\n",dms);
      return(-1);
   }
   else { /* first character is "+" or "-" */
      if(strncmp(dms,"+",1)==0){
         sign=1;
      }
      else{
         sign=-1;
      }
   }

   strcpy(buf,dms+1);
   strncpy(buf+2," ",1);
   sscanf(buf,"%d",&dd);

   strcpy(buf,dms+3);
   strncpy(buf+2," ",1);
   sscanf(buf,"%d",&mm);

   strcpy(buf,dms+5);
   sscanf(buf,"%f",&ss);
     
   *d=dd+(mm/60.0)+(ss/3600.0);

   if(sign<1)*d=-*d;

   return(0);
}


   
   

