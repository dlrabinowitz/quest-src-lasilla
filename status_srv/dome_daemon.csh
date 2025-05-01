#!/bin/tcsh
#
# dome_daemon.csh
#
# check la silla domes status using weather_srv on quest16.
# Check schmidt dome status by reading tcs.status file which
# regenerated each time questctl reads a reply from the TCS.
# If the la silla domes are closed and the schmidt is open,
# then closed the schmidt
#
# DLR 2009 Jul 24
#
set FAKE_TEST = 0
if ( $FAKE_TEST == 1 ) then
  set TCS_FILE = "/home/observer/quest-src-lasilla/fake_tcs.status"
  set PID_FILE = "/home/observer/logs/fake_questctl.pid"
  set COMMAND_FILE = "/home/observer/status_srv/fake_dome_daemon.command"
  set SIGNAL_FILE =  "/home/observer/logs/fake_questctl.signal"
  set dome_log = "fake_dome_daemon.log"
  set temp_file = "fake_dome_daemon.tmp"
  set MAIN_LOOP_DELAY = 10
else
  set TCS_FILE = "/home/observer/quest-src-lasilla/tcs.status"
  set PID_FILE = "/home/observer/logs/questctl.pid"
  set COMMAND_FILE = "/home/observer/status_srv/dome_daemon.command"
  set SIGNAL_FILE =  "/home/observer/logs/questctl.signal"
  set dome_log = "dome_daemon.log"
  set temp_file = "dome_daemon.tmp"
  set MAIN_LOOP_DELAY = 60
endif

if ( ! -e $PID_FILE ) then
   echo "$PID_FILE does not exist. Exiting" 
endif
if ( ! -e $TCS_FILE ) then
   echo "$TCS_FILE does not exist. Exiting" 
endif

#setup open and close dome aliases that use signals sent to process 
#id of questctl. Also make alias to check if questctl is running

set CLOSEDOME_SIGNAL = 10 
set STOWDOME_SIGNAL = 12 
set OPENDOME_SIGNAL = 16 
#
# closedome_direct by passes questctl to close the dome. Necessary is
# questctl has crashed or is not running
alias closedome_direct '/home/observer/bin/closedome'
#
# domestatus_direct needee when questctl not running and updating 
# TCS status file
alias domestatus_direct  '/home/observer/bin/domestatus |& tail -n 1'
#
#
alias domestatus 'set l0 = `cat $TCS_FILE | cut -c 1-125`; if ( $#l0 >= 15 ) echo $l0[$#l0] ; if ($#l0 < 15) echo -1'
#
# use this aliass to check if the sun is up or not
alias check_sunup 'set l0 = `/home/observer/bin/sunup |& tail -n 1`; echo $l0'
#
#
# set TEST to 1 to prevent any dome operations, and issue debug statements
# instead
set TEST = 0
set VERBOSE = 1
#
# number of times to try to read dome status
set NUM_STATUS_TRIES = 10
#
# number of seconds to wait between retries
set RETRY_DELAY_TIME = 30
#
# number of time to try getting close status  (20 x 30 seconds total)
set NUM_CLOSE_STATUS_TRIALS = 20
#
# number of time to try getting open status  (10 * 30 seconds total)
set NUM_OPEN_STATUS_TRIALS = 10
#
if ( $FAKE_TEST ) then
  set RETRY_DELAY_TIME = 10
  set NUM_CLOSE_STATUS_TRIALS = 3 
  set NUM_OPEN_STATUS_TRIALS = 3 
endif
#
# try 12 times to read signal reponse, with a 10 second delay between attempts
set SIGNAL_WAIT_TIME =  10
set NUM_SIGNAL_QUERIES = 12
set NUM_SIGNAL_RETRIES = 3
#
set iteration = 0
#
date >! $dome_log
echo "starting dome_daemon" >> $dome_log
#
# check weather and dome states periodically.
# If La Silla domes close, send closedome command
#
#
while (1)

# check if sun is up. If so and dome is open, close dome and exit

   if ( $FAKE_TEST == 1 ) then
      set sunup_flag = 0
   else
      if ( `check_sunup` ) then
         echo `date` " sun is up " >>& $dome_log
         set sunup_flag = 1
      else
         echo `date` " sun is down " >>& $dome_log
         set sunup_flag = 0
      endif
   endif

# see if there is a commanded state in COMMAND_FILE

set command = "NOTHING"
if ( -e $COMMAND_FILE ) then
  set l = `cat $COMMAND_FILE | tail -n 1`
  if ( $#l >= 1 ) then
      set command = $l[1]
      echo `date` " command $command found in command file" >>& $dome_log
      echo $l >>& $dome_log
  endif
endif

# see if schmidt dome is open
#
#
  if ($VERBOSE == 1 ) then
    echo `date` " checking dome status" >> $dome_log
  endif
 
  set questctl_pid = `cat $PID_FILE`
  set questctl_running = `ps -aef | grep -e $questctl_pid | grep -ve "grep" | wc -l`
  if ( $questctl_running ) then
     set l1 = `domestatus`
     set i = 1
     while ( $l1 == -1 && $i < 10)
        @ i = $i + 1
        sleep 1
        set l1 = `domestatus`
     end
  else
     echo `date` "questctl not running" >> $dome_log
     if ( $FAKE_TEST == 1 ) then
        echo "domestatus_direct not allowed in fake test"
        exit
     endif
     domestatus_direct >& $temp_file
     cat $temp_file >> $dome_log
     set l2 = `cat $temp_file | tail -n 1 | grep -e "dome open" | wc -l`
     set l3 = `cat $temp_file | tail -n 1 | grep -e "failed" | wc -l`
     set l1 = 0
     @ l1 = $l2 - $l3
  endif

  if ( $l1 == 1 ) then
     set eso_schmidt_closed = 0
  else if ( $l1 == 0 ) then
     set eso_schmidt_closed = 1
  else
     echo `date` " can't get schmidt dome status" >>& $dome_log
     echo `date` " TCS status: " `cat $TCS_FILE` >>& $dome_log
     set eso_schmidt_closed = -1
  endif
#
#
  if ($VERBOSE == 1 ) then
    echo `date` "  schmidt dome closed status is " $eso_schmidt_closed >> $dome_log
    cat $TCS_FILE >> $dome_log
  endif
#
# see if lasilla domes are open
#
  if ($VERBOSE == 1 ) then
    echo `date` " checking lasilla domes" >> $dome_log
  endif
#
# check lasilla dome status up to NUM_STATUS_TRIES times to get valid result

#
  if ( $FAKE_TEST == 1 ) then
     set lasilla_domes_open = 1
     set l2 = "faking lasilla domes open 1"
  else
    set error_flag = 1
    set iteration = 1
    while ( $error_flag == 1 && $iteration < $NUM_STATUS_TRIES )
      set l2 = `weather` 
      if ( $#l2 != 19 || $l2[19] == -1 ) then
        echo `date` "iteration $iteration : error checking lasilla dome status" >> $dome_log
        sleep $RETRY_DELAY_TIME
        @ iteration = $iteration + 1
      else
        set error_flag = 0
      endif
    end

    if ( $error_flag ) then
      echo `date` "error reading lasilla dome status after $iteration iterations" >> $dome_log 
      set lasilla_domes_open = -1
    else if ( $l2[19] != 1 ) then
      set lasilla_domes_open = 0
    else
      set lasilla_domes_open = 1
    endif    
  endif
#
  if ($VERBOSE == 1 ) then
    echo `date` "  lasilla dome status is " $l2 >> $dome_log
    echo `date` " schmidt closed status is " $eso_schmidt_closed  >> $dome_log
    echo `date` " lasilla domes open status is " $lasilla_domes_open  >> $dome_log
  endif
#
#
# if the lasilla domes are still not open, or the sun is up, or the latest command is "CLOSE", 
# and the schmidt is not closed, then close the schmidt dome
#
  if ( $eso_schmidt_closed != 1 && ( $lasilla_domes_open != 1 || $sunup_flag == 1 || $command == "CLOSE"))  then
     if ( $lasilla_domes_open != 1 ) then
       echo `date` " schmidt dome is not closed and lasilla domes are not open" >> $dome_log
     else if ( $command == "CLOSE" ) then
       echo `date` " schmidt dome is not closed and command is $command" >> $dome_log
     endif
     echo `date` " weather and lasilla dome status: $l2" >>& $dome_log
     echo `date` "schmidt dome status: `cat $TCS_FILE`" >>& $dome_log
     if ( $TEST == 1 ) then
        echo `date` "test: would close dome now" >>& $dome_log
     else 
        echo `date` " closing schmidt dome" >>& $dome_log
        set questctl_pid = `cat $PID_FILE`
        set questctl_running = `ps -aef | grep -e $questctl_pid | grep -ve "grep" | wc -l`
        if ( $questctl_running == 1 ) then
################
           set num_signal_attempts = 1
           set signal_received = 0
           set expected_response = "CLOSE"
           while ( $num_signal_attempts <= $NUM_SIGNAL_RETRIES && $signal_received == 0 )
             echo `date` " iteration $num_signal_attempts : sending signal to close the schmidt dome" >>& $dome_log
	     set t1 = `date +"%s"`
	     @ t1 = $t1 - 3
             kill -$CLOSEDOME_SIGNAL `cat $PID_FILE` >>& $dome_log
             echo `date` " done sending close dome signal" >> $dome_log
#
             set iteration = 1
             while ( $signal_received == 0 && $iteration <= $NUM_SIGNAL_QUERIES )
   	       echo `date` " iteration: $iteration  waiting to see if $expected_response signal is received" >> $dome_log
	       set l = `cat $SIGNAL_FILE |& grep -e "$expected_response"`
               if  ( $#l != 2 ) then
                  echo `date` " $expected_response signal still not received" >> $dome_log
	          set t2 = 0
               else 
                  set t2 = $l[2]
	          if ( $t2 > $t1 ) then
                     set signal_received = 1
		     echo `date` "$expected_response signal received $l" >> $dome_log
		  else
		     echo `date` "$expected_response signal not recent : $l" >> $dome_log
		     echo `date` " t1 = $t1  t2 = $t2" >> $dome_log
                  endif
               endif
               if ( $signal_received == 0 ) then
	          @ iteration = $iteration + 1
	          sleep $SIGNAL_WAIT_TIME
               endif
             end
             @ num_signal_attempts = $num_signal_attempts + 1
           end
           if ( $signal_received == 0 ) then
             echo `date` " $expected_response signal not received after $NUM_SIGNAL_RETRIES attempts" >> $dome_log
#            set iteration to NUM_CLOSE_STATUS_TRIALS + 1 to prevent wait for dome open
             @ iteration = $NUM_CLOSE_STATUS_TRIALS + 1
           else
             set iteration = 1
           endif
#################
        else
           echo `date` "questctl not running" >> $dome_log
           if ( $FAKE_TEST == 1 ) then
             echo "closedome_direct not allowed in fake test"
             exit
           endif
           echo `date` "  sending direct close dome command" >> $dome_log
           closedome_direct >>& $dome_log
           echo `date` " done sending direct close dome command" >> $dome_log
           set iteration = 1
        endif
        while ( $eso_schmidt_closed != 1 && $iteration <= $NUM_CLOSE_STATUS_TRIALS )
#
          echo `date` " iteration $iteration : checking if dome has closed yet" >> $dome_log
#
#         check to see if dome is closed

          set questctl_pid = `cat $PID_FILE`
          set questctl_running = `ps -aef | grep -e $questctl_pid | grep -ve "grep" | wc -l`
          if ( $questctl_running ) then
             set l1 = `domestatus`
             set i = 1
             while ( $l1 == -1 && $i < 10)
                @ i = $i + 1
                sleep 1
                set l1 = `domestatus`
             end   
          else
            if ( $FAKE_TEST == 1 ) then
              echo "domestatus_direct not allowed in fake test"
              exit
            endif
            domestatus_direct >& $temp_file
            cat $temp_file >> $dome_log
            set l2 = `cat $temp_file | tail -n 1 | grep -e "dome open" | wc -l`
            set l3 = `cat $temp_file | tail -n 1 | grep -e "failed" | wc -l`
            set l1 = 0
            @ l1 = $l2 - $l3
          endif

          if ( $l1 == 1 ) then
             set eso_schmidt_closed = 0
          else if ( $l1 == 0 ) then
             set eso_schmidt_closed = 1
          else
             echo `date` " can't get schmidt dome status" >>& $dome_log
             echo `date` " TCS status: " `cat $TCS_FILE` >>& $dome_log
             set eso_schmidt_closed = -1
          endif

#
	  date >>& $dome_log
          if ( $eso_schmidt_closed == 1 )then
            echo `date` "iteration $iteration : schmidt dome now closed" >> $dome_log
          else
            echo `date` "iteration $iteration : schmidt dome has not closed yet" >> $dome_log
            echo `date` "status is : " `cat $TCS_FILE` >> $dome_log
            echo `date` "iteration $iteration : waiting $RETRY_DELAY_TIME seconds" >> $dome_log
            sleep $RETRY_DELAY_TIME
          endif

          @ iteration = $iteration + 1
        end
     endif
  endif
#
# Otherwise, if the lasilla domes are  open and the command is "OPEN", and questctl is running, 
# and the sun is down, and the schmidt is  closed, then open the schmidt dome
#
  set questctl_pid = `cat $PID_FILE`
  set questctl_running = `ps -aef | grep -e $questctl_pid | grep -ve "grep" | wc -l`
  if ( $sunup_flag == 0 && $questctl_running == 1 && $eso_schmidt_closed != 0 &&  $lasilla_domes_open == 1 && $command == "OPEN" ) then
     echo `date` " sun is down, schmidt dome is not open, lasilla domes are open, and command is $command " >> $dome_log
     echo `date` " weather and lasilla dome status: $l2" >>& $dome_log
     echo `date` "schmidt dome status: `cat $TCS_FILE`" >>& $dome_log
     if ( $TEST == 1 ) then
        echo `date` "test: would open dome now" >>& $dome_log
     else
################
        set num_signal_attempts = 1
        set signal_received = 0
        set expected_response = "OPEN"
        while ( $num_signal_attempts <= $NUM_SIGNAL_RETRIES && $signal_received == 0 )
          echo `date` " iteration $num_signal_attempts : sending signal to open the schmidt dome" >>& $dome_log
	  set t1 = `date +"%s"`
	  @ t1 = $t1 - 3
          kill -$OPENDOME_SIGNAL `cat $PID_FILE` >>& $dome_log
          echo `date` " done sending open dome signal" >> $dome_log
#
          set iteration = 1
          while ( $signal_received == 0 && $iteration <= $NUM_SIGNAL_QUERIES )
   	    echo `date` " iteration: $iteration  waiting to see if $expected_response signal is received" >> $dome_log
	    set l = `cat $SIGNAL_FILE |& grep -e "$expected_response"`
            if  ( $#l != 2 ) then
              echo `date` " $expected_response signal still not received" >> $dome_log
	      set t2 = 0
            else 
              set t2 = $l[2]
	      if ( $t2 > $t1 ) then
                 set signal_received = 1
		 echo `date` " $expected_response signal received $l" >> $dome_log
              endif
            endif
            if ( $signal_received == 0 ) then
	      @ iteration = $iteration + 1
	      sleep $SIGNAL_WAIT_TIME
            endif
          end
          @ num_signal_attempts = $num_signal_attempts + 1
        end

        if ( $signal_received == 0 ) then
          echo `date` " $expected_response signal not received after $NUM_SIGNAL_RETRIES attempts" >> $dome_log
#         set iteration to 1 + NUM_OPEN_STATUS_TRIALS to stop wait for dome open
          @ iteration = $NUM_OPEN_STATUS_TRIALS + 1
        else 
          set iteration = 1  
        endif
#######################
        while ( $eso_schmidt_closed != 0 && $iteration <= $NUM_OPEN_STATUS_TRIALS )
#
          echo `date` " iteration $iteration : checking if  dome has opened" >> $dome_log
#
          set l1 = `domestatus`
          set i = 1
          while ( $l1 == -1 && $i < 10)
              @ i = $i + 1
              sleep 1
              set l1 = `domestatus`
          end   
 
          if ( $l1 == 1 ) then
             set eso_schmidt_closed = 0
          else if ( $l1 == 0 ) then
             set eso_schmidt_closed = 1
          else
             echo `date` " can't get schmidt dome status" >>& $dome_log
             echo `date` " TCS status: " `cat $TCS_FILE` >>& $dome_log
             set eso_schmidt_closed = -1
          endif

 #
	  date >>& $dome_log
          if ( $eso_schmidt_closed == 0 )then
            echo `date` "iteration $iteration : schmidt dome now open" >> $dome_log
          else
            echo `date` "iteration $iteration : schmidt dome is not yet open" >> $dome_log
            echo `date` "status is : " `cat $TCS_FILE` >> $dome_log
            echo `date` "iteration $iteration : waiting $RETRY_DELAY_TIME seconds" >> $dome_log
            sleep $RETRY_DELAY_TIME
          endif

          @ iteration = $iteration + 1
        end
     endif
  endif

#
# If the the sun is down, questctl is running, the schmidt dome is closed, the lasilla domes 
# are open, but the command is not "OPEN", just echo a warning.
#
  set questctl_pid = `cat $PID_FILE`
  set questctl_running = `ps -aef | grep -e $questctl_pid | grep -ve "grep" | wc -l`
  if ( $sunup_flag == 0 && $questctl_running == 1 && $eso_schmidt_closed != 0 &&  $lasilla_domes_open == 1 && $command != "OPEN" ) then
     echo `date` " sun is down, schmidt dome is not open, lasilla domes are open, but command is not OPEN: $command " >> $dome_log 
     echo `date` " weather and lasilla dome status: $l2" >>& $dome_log
     echo `date` "schmidt dome status: `cat $TCS_FILE`" >>& $dome_log
  endif
#
#
  if ( $VERBOSE == 1 ) then
     echo `date` "weather and lasilla dome status: $l2" >>& $dome_log
     echo `date` "schmidt dome status: `cat $TCS_FILE`" >>& $dome_log
     echo `date` "dome status ok" >>& $dome_log
  endif

  if ( $sunup_flag ) then
     echo `date` "Sun is up. Exiting" >>& $dome_log
     exit
  endif

  echo `date` "sleeping $MAIN_LOOP_DELAY  s" >>& $dome_log
  sleep $MAIN_LOOP_DELAY
end

