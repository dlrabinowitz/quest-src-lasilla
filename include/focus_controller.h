/**************************************************************************
 * $RCSfile: focus_controller.h,v $
 * $Revision: 1.6 $
 * $Date: 2009/07/23 20:57:09 $
 * Author - Erik Hovland
 * Class definition for the focus controller object. And it's
 * it's corresponding command string class.
 **************************************************************************/
#ifndef INC_FOCUS_CONTROLLER_H
#define INC_FOCUS_CONTROLLER_H

#include <focus_drv.h>

enum FOCUS_CMDS { FOCUS_CMD_CTLR_STS=0,FOCUS_CMD_OS_STS=1,FOCUS_CMD_MP_STS=2,
				  FOCUS_CMD_MP_ON=3,FOCUS_CMD_MP_OFF=4,FOCUS_CMD_OS_CMD=5 };

class focus_command; // forward declaration
/**************************************************************************
 * focus
 **************************************************************************/
class focus_controller {
	public:
		focus_controller();
		focus_controller(const double offset);
		~focus_controller();
		double get_offset();
		int set_offset(const double offset);
		int get_controller_status();
		int get_motor_status();
		int motor_poweron();
		int motor_poweroff();
		int motor_powercycle(int on_off);
	private:
		int* focus_comm_fd;		// file descriptor to focus controller
								// serial port
		char* sendcmd(const focus_command command);
};

/**************************************************************************
 * focus_command - helper class to ease the use of the slightly complex
 * but short list of focus commands. This class should be able to easily
 * construct a command string by passing some easy types to it like
 * double for the offset or integers or characters.
 **************************************************************************/

class focus_command {
	public:
		focus_command();
		focus_command(double offset);
		~focus_command();

		char* get_command_string();
		void set_type(int command_type);// sets type of command string
		int set_offset(double offset);	// sets offset of offset command
										// string.

	private:
		char* command_string;
};
#endif	// INC_FOCUS_CONTROLLER_H
