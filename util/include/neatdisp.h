#ifndef _NEATDISP_H_
#define _NEATDISP_H_
/* neatdisp.h
 * @(#)neatdisp.h	1.3 18 Dec 1995
 *
 * Definition of the NEAT display class.
 * Steve Groom, JPL
 * 8/28/95
 */

class neatdisp
{
private:
    pid_t pid;		// PID of display process
    int rows;		// size of image display
    int cols;
    int start();	// start (restart) viewer process
    char pidfilename[NEAT_FILENAMELEN];
public:
    neatdisp(int disp_rows, int disp_cols);
    ~neatdisp();

    int fail;

    int update(char *image_file);
};
#endif /* _NEATDISP_H_ */
