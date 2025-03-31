/**************************************************************************
 * $RCSfile: faketele_controller.h,v $
 * $Revision: 1.6 $
 * $Date: 2009/07/23 20:57:09 $
 * Author - Erik Hovland
 *
 * Class declaration for using the new telescope controller at palomar
 **************************************************************************/

#ifndef INC_TELESCOPE_CONTROLLER_H
#define INC_TELESCOPE_CONTROLLER_H

#include <faketele_drv.h>
#include <point.h>
#include <tm_angles.h>

enum TELESCOPE_CMDS {
/* command name				value */
/* ------------				----- */
   TELESCOPE_CMD_TAKE_CTLR=		0,
   TELESCOPE_CMD_REL_CTLR=		1,
   TELESCOPE_CMD_STS=			2,
   TELESCOPE_CMD_STRTK=			3,
   TELESCOPE_CMD_STRTK_LOAD=		4,
   TELESCOPE_CMD_STRTK_RCL=		5,
   TELESCOPE_CMD_STOW=			6,
   TELESCOPE_CMD_STOP=			7,
   TELESCOPE_CMD_LINK_TO=		8,
   TELESCOPE_CMD_LINK_CK=		9,
   TELESCOPE_CMD_FOCUS=			10,
   TELESCOPE_CMD_FCS_STOP=		11,
   TELESCOPE_CMD_WTHR_STS=		12,
   TELESCOPE_CMD_ACK_FLT=		13,
   TELESCOPE_CMD_STS_MSG_REQ=		14,
   TELESCOPE_CMD_FLT_REQ=		15,
   TELESCOPE_CMD_ACU_STS_REQ=		16,
   TELESCOPE_CMD_CCU_STS_REQ=		17,
   TELESCOPE_CMD_SLAVE_DOME=		18,
   TELESCOPE_CMD_SLAVE_WINDSCR=		19,
   TELESCOPE_CMD_OPEN_DOME=		20,
   TELESCOPE_CMD_CLOSE_DOME=		21,
   TELESCOPE_CMD_MAX_WIND_SPEED_REQ=	22,
   TELESCOPE_CMD_DP_TRIGGER_REQ=	23
};

enum FCS_LIMITS { MIN_FCS = -50, MAX_FCS = 50 };

// Enums follow the Telescope Interface Spec, see that document for more
// information. Specifically the STATUS REQUEST (sec 8.7) and BINARY
// STATUS REQUEST (sec. 8.6) commands.

// tcu_modes correspond to each mode available in the mode string returned
// by the STAT command
enum tcu_modes {
/* mode name		array entry */
/* ---------		----------- */
   funm =		0,
   funsubm =		1,
   focm =		2,
   focsubm =		3,
   domem =		4,
   domesubm =		5,
   windscrm =		6,
   windscrsubm =	7,
   tot_modes =		8
};

// each possible value of the fundamental modes is enumerated here. These
// values are all given in table 8.7-1. As are the corresponding submodes
enum fun_modes {
/* fundamental mode name	value */
/* ---------------------	----- */
   stopm =			0,
   presetm =			2,
   manrm =			7,
   manpm =			8,
   maintm =			6,
   posdm =			1,
   stowm =			9,
   poshm =			29,
   strkm =			12
};
enum submodes {
/* fundamental submode name	value */
/* ------------------------	----- */
   resetsm =			0,
   transsm =			1,
   actsm =			2,
   offtsm =			3,
   stopsm =			4,
   disabledsm =			5,
   pendsm =			6,
   waitsm =			7,
   finism =			8,
   poshsm =			9
};

enum focmodes {
/* focus mode name	value */
/* ---------------	----- */
   fstopm =		50,
   fposdm =		51,
   fmaintm =		53,
   fmanrm =		54
};

enum fsubmodes {
/* focus submode name	value */
/* ------------------	----- */
   fofftsm =		0,
   factsm =		1
};

enum dmodes {
/* dome mode name	value */
/* --------------	----- */
   dstopm =		70,
   dposdm =		71,
   dstowm =		72,
   dslavem =		73,
   dmaintm =		53
};

enum wmodes {
/* wind screen modes	value */
/* -----------------	----- */
   wstopm =		70,
   wposdm =		71,
   wstowm =		72,
   wslavem =		73,
   wmaintm =		53
};

// These masks correspond to the flags of either status or fault which are
// important to the operations of the telescope. Each fault or status is
// described in the manual in section 8.7 and is the long list of bytes
// and flags.
enum fns_masks {
/* fault or status flag			mask */
/* --------------------			---- */
   dome_open_mask =			4,
   dome_close_mask =			4,
   dome_mismatch_mask =			8,
   acu_e_stop_mask =			1,
   psu_ilock_mask =			1,
   tel_gen_ilock_mask =			1,
   ha_drive_fault_mask =		1,
   ha_cw_prelim_mask =			2,
   dec_drive_fault_mask =		1,
   ha_ccw_prelim_mask =			2,
   dec_up_prelim_mask =			2,
   ha_motor_overtemp_mask =		4,
   dec_down_prelim_mask =		2,
   dec_motor_overtemp_mask =		4,
   ccu_acu_link_down_mask =		1,
   dome_bc_link_down_mask =		128,
   windscr_encoder_fault_mask =		1,
   ha_encoder_fault_mask =		16,
   dec_encoder_fault_mask =		32,
   focus_encoder_fault_mask =		64,
   dome_encoder_fault_mask =		128,
   ha_tach_encoder_error_mask =		1,
   dec_tach_encoder_error_mask =	2,
   ha_brake_fault_mask =		8,
   dec_brake_fault_mask =		8,
   pdu_cb_contactor_open_mask =		4,
   ccu_db_empty_mask =			16,
   ha_dec_ik220_init_failed_mask =	128,
   cmd_region_ha_plus_mask =		1,
   cmd_region_ha_minus_mask =		2,
   cmd_region_dec_plus_mask =		4,
   cmd_region_dec_minus_mask =		8,
   ha_plus_sw_limit_mask =		16,
   ha_minus_sw_limit_mask =		32,
   dec_plus_sw_limit_mask =		64,
   dec_minus_sw_limit_mask =		128,
   irig_signal_loss_mask =		8,
   tel_below_hz_mask =			4,
   acu_ccu_link_down_mask =		2
};

// these 'bytes' are really the subscript values to the either the ccu
// or acu status arrays that ccu_status() and acu_status(). Since they
// reference arrays you will notice that they are off by one (2 instead of 3)
// from the 'byte' number in section 8.7
enum ccu_stat_bytes {
/* status or fault byte			subscript value */
/* --------------------			--------------- */
   ccu_ha_drive_fault_byte =		0,
   ccu_ha_cw_prelim_byte =		0,
   ccu_dec_drive_fault_byte =		1,
   ccu_ha_ccw_prelim_byte =		1,
   ccu_dome_open_byte =			2,
   ccu_dome_close_byte =		3,
   ccu_acu_e_stop_byte =		3,
   ccu_psu_ilock_byte =			4,
   ccu_dec_up_prelim_byte =		4,
   ccu_ha_motor_overtemp_byte =		4,
   ccu_tel_gen_ilock_byte =		5,
   ccu_dec_down_prelim_byte =		5,
   ccu_dec_motor_overtemp_byte =	5,
   ccu_ccu_acu_link_down_byte =		16,
   ccu_dome_bc_link_down_byte =		16,
   ccu_windscr_encoder_fault_byte =	18,
   ccu_ha_encoder_fault_byte =		18,
   ccu_dec_encoder_fault_byte =		18,
   ccu_focus_encoder_fault_byte =	18,
   ccu_dome_encoder_fault_byte =	18,
   ccu_ha_tach_encoder_error_byte =	19,
   ccu_dec_tach_encoder_error_byte =	19,
   ccu_ha_brake_fault_byte =		2,
   ccu_dec_brake_fault_byte =		3,
   ccu_pdu_cb_contactor_open_byte =	6,
   ccu_ccu_db_empty =			16,
   ccu_ha_dec_ik220_init_failed_byte =	22
};

// another note, the tcu/acu is written so that the whole bunch of acu and ccu
// status are all jumbled into one big string of bytes, 70 bytes. But the
// commands to access either the ccu or acu split this 70 byte string into
// two pieces, the acu 40 byte piece and ccu 30 byte piece. So the document
// talks about all 70 bytes as one, but the methods report the pieces. This is
// easy for the ccu since you just subtract one from the byte address in the
// document. For the acu it is also easy but a little different, It is
// (<doc byte @> - 30) - 1, which is 'subtract off the 30 ccu bytes' then
// 'subtract 1' because the status is given to you as an array which starts
// at 0 and not 1 like the document. For example, notice below that the dome
// mismatch byte is given as 38, but in the document it is 69, using our
// formula you get - (69 -30) - 1 = 38, viola.

enum acu_stat_bytes {
/* status or fault byte		subscript value */
/* --------------------		--------------- */
   acu_dome_mismatch_byte =		38,
   acu_cmd_region_ha_plus_byte =	0,
   acu_cmd_region_ha_minus_byte =	0,
   acu_cmd_region_dec_plus_byte =	0,
   acu_cmd_region_dec_minus_byte =	0,
   acu_ha_plus_sw_limit =		0,
   acu_ha_minus_sw_limit =		0,
   acu_dec_plus_sw_limit =		0,
   acu_dec_minus_sw_limit =		0,
   acu_irig_signal_loss_byte =		28,
   acu_tel_below_hz_byte =		38,
   acu_acu_ccu_link_down_byte =		7
};

// the fault return values for fault_status()
enum f_stat_vals {
/* fault name			value */
/* ----------			----- */
   f_no_fault =			0,
   f_acu_estop =		-1,
   f_psu_ilock =		-2,
   f_tel_gen_ilock =		-3,
   f_ha_drive_fault =		-4,
   f_ha_cw_prelim =		-5,
   f_dec_drive_fault =		-6,
   f_ha_ccw_prelim =		-7,
   f_dec_up_prelim =		-8,
   f_ha_motor_overtemp =	-9,
   f_dec_down_prelim =		-10,
   f_dec_motor_overtemp =	-11,
   f_ccu_acu_link_down =	-12,
   f_dome_bc_link_down =	-13,
   f_windscr_encoder_fault =	-14,
   f_ha_encoder_fault =		-15,
   f_dec_encoder_fault =	-16,
   f_focus_encoder_fault =	-17,
   f_dome_encoder_fault =	-18,
   f_ha_tach_encoder_error =	-19,
   f_dec_tach_encoder_error =	-20,
   f_ha_brake_fault =		-21,
   f_dec_brake_fault =		-22,
   f_pdu_cb_contactor_open =	-23,
   f_ccu_db_empty =		-24,
   f_ha_dec_ik220_init_failed =	-25,
   f_cmd_region_ha_plus =	-26,
   f_cmd_region_ha_minus =	-27,
   f_cmd_region_dec_plus =	-28,
   f_cmd_region_dec_minus =	-29,
   f_ha_plus_sw_limit =		-30,
   f_ha_minus_sw_limit =	-31,
   f_dec_plus_sw_limit =	-32,
   f_dec_minus_sw_limit =	-33,
   f_irig_signal_loss =		-34,
   f_tel_below_hz =		-35,
   f_acu_ccu_link_down =	-36,
   f_comm_failed =		-50
};

// the return values for dome_status()
enum d_stat_vals {
/* dome stat name		value */
/* --------------		----- */
   d_success =			0,
   d_comm_failed =		-8
};

// the return vaules for dome_shutter_status()
enum ds_stat_vals {
/* dome shutter stat name	value */
/* ----------------------	----- */
   ds_open =			1,
   ds_closed =			0,
   ds_stat_unknown =		-1,
   ds_comm_failed =		-2
};

const unsigned int acu_stat_size = 40;
const unsigned int ccu_stat_size = 30;
const int dome_shutter_timeout = 2400;

class telescope_command; // forward declaration

// simple vector to use to send and receive points.
// The units are in degrees (for both).
//struct point {
//	double ra;
//	double dec;
//};

struct weather_data {
	double temp;
	double humidity;
	double wind_speed;
	double wind_direction;
	double dew_point;
};

struct weather_triggers {
    double max_wind_speed;
    double dew_point_tolerance;
};

struct tcu_status {
    double hap;
    double decp;
    double focusp;
    double domep;
    double windscreenp;
    unsigned short mode[8];
};

struct d_status {
    unsigned short dome_mode;
    unsigned short dome_submode;
    double domep;
    int dome_mismatch_flag;
};

struct ws_status {
    unsigned short windscr_mode;
    unsigned short windscr_submode;
    double windscrp;
};
/**************************************************************************
 * CLASS: telescope_controller
 *
 * DESCRIPTION:
 *   This class encapsulates all the commands that should be necessary to
 *   control the telescope. The telescope is interfaced through the
 *   serial port to another computer which actually does all of the
 *   control. The operation should contain all commands that exist in the
 *   telmount class so that we can use the telmount interface for external
 *   commanding of the telescope (telmgr and obsmgr)
 **************************************************************************/
class telescope_controller {
	public:
		enum TC_RESPONSES { TC_NACK = 0, TC_ACK = 1, TC_FAIL = -1};
		telescope_controller();
		telescope_controller(char*);
		~telescope_controller();
//		int is_ready();
		int take_control();
//		int check_point(const point&);
		int point(struct point&);
		void get_position(struct point&);
		int stop();
		int stow();
		int status();
		char* faults();
		double get_focus();
		int set_focus(const double);
		int focus_status();
		int stat_req(struct tcu_status&);
		int stop_focus();
		int open_dome_shutter();
		int close_dome_shutter();
		int slave_dome();
		int slave_windscr();
		int dome_status(struct d_status&);
		int windscr_status(struct ws_status&);
		int dome_shutter_status();
		int acu_status(unsigned short []);
		int ccu_status(unsigned short []);
		int fault_status();
		struct weather_data get_weather_data();
		int get_weather_triggers(struct weather_triggers*);
		int set_link_timeout(const double);
		int ack_fault();
		double get_link_timeout();
		char* sendcmd(telescope_command*);
		void setcoords(const double&, const double&);

	private:
		int *telescope_comm_fd;
		char* class_name;
		bool has_control;
		void dummy_load_weather(struct weather_data*);
		struct point current_position;
		double pos_lon;
		double pos_lat;
		azel_ang cur_azel;
//		double rate;
};

class telescope_command {
	public:
		telescope_command();
		telescope_command(struct point&);
		telescope_command(const double);
		telescope_command(int);
		~telescope_command();

		char* get_command_string();
		void set_command_type(int);
		int set_point(struct point&);
		int set_focus(const double);
		int set_timeout(const double);

	private:
		char* command_string;
		int command_type;
};

#endif //INC_TELESCOPE_CONTROLLER_H
