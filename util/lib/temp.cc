/**************************************************************************
 * METHOD: telescope_controller::telescope_status()
 *
 * DESCRIPTION:
 *   Returns a complete status report for the tescope using all the
 *   bits of information in the TCU response
 * 
 **************************************************************************/
int telescope_controller::telescope_status() {
    /* There is no way to know if we are tracking except to see that RA and Dec are constant
       and that the motion_status_mode is stable. So make two calls to get status, separated
       by short delay (3 sec) and see if ra or dec has changed in the mean time */
    struct tcu_status cur_stat;
    mode_index index;


    if (stat_req(cur_stat) == -1) {
	fprintf(stderr, "%s: status request failed\n", class_name);
	return -1;
    }

    switch(cur_stat.dome_shutter_state){
    case ds_open:
     	fprintf(stdout, "Dome Status : %s\n", "OPEN");
	break;
    case ds_closed:
     	fprintf(stdout, "Dome Status : %s\n", "CLOSED");
	break;
    case ds_stat_unknown:
     	fprintf(stdout, "Dome Status : %s\n", "UNKNOWN");
	break;
    default:
     	fprintf(stdout, "Dome Status : %s\n", "UNKNOWN");
    }

    fprintf(stdout,"Telescope RA: %f\n",cur_stat.rap);
    fprintf(stdout,"Telescope Dec: %f\n",cur_stat.decp);
    fprintf(stdout,"Telescope HA: %f\n",cur_stat.hap);
    fprintf(stdout,"Telescope LST: %f\n",cur_stat.lst);
    fprintf(stdout,"Telescope JD: %f\n",cur_stat.jd);
    fprintf(stdout,"Telescope Epoch: %f\n",cur_stat.epoch);
    fprintf(stdout,"Focus: %f\n",cur_stat.focusp);
    fprintf(stdout,"Dome Position: %f\n",cur_stat.domep);

    switch(cur_stat.mode[wobble_mode]){
    case 0:  
        fprintf(stdout,"Wobble Status: %s\n","OFF_BEAM");
        break;
    case 1:  
        fprintf(stdout,"Wobble Status: %s\n","ON_BEAM1");
        break;
    case 2:  
        fprintf(stdout,"Wobble Status: %s\n","ON_BEAM2");
        break;
    default:
        fprintf(stdout,"Wobble Status: %s\n","UNKNOWN");
    }

    switch(cur_stat.mod[horiz_limit_mode]){
    case 0:  
        fprintf(stdout,"Horizon Limit: %s\n","OUT_OF_LIMIT");
        break;
    case 1:  
        fprintf(stdout,"Horizon Limit: %s\n","IN_LIMIT");
        break;
    default:
        fprintf(stdout,"Horizon Limit: %s\n","UNKNOWN");
    }

    switch(cur_stat.mod[ra_limit_mode]){
    case 0:  
        fprintf(stdout,"RA Limit: %s\n","OUT_OF_LIMIT");
        break;
    case 1:  
        fprintf(stdout,"RA Limit: %s\n","IN_LIMIT");
        break;
    default:
        fprintf(stdout,"RA Limit: %s\n","UNKNOWN");
    }

    switch(cur_stat.mod[dec_limit_mode]){
    case 0:  
        fprintf(stdout,"Dec Limit: %s\n","OUT_OF_LIMIT");
        break;
    case 1:  
        fprintf(stdout,"Dec Limit: %s\n","IN_LIMIT");
        break;
    default:
        fprintf(stdout,"Dec Limit: %s\n","UNKNOWN");
    }

    switch(cur_stat.mod[drive_status_mode]){
    case 0:  
        fprintf(stdout,"Servos: %s\n","ENABLED");
        break;
    case 1:  
        fprintf(stdout,"Dec Limit: %s\n","DISABLED");
        break;
    default:
        fprintf(stdout,"Dec Limit: %s\n","UNKNOWN");
    }


    return(0);

}
/******************************************************************************/
