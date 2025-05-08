#!/bin/tcsh
#
# closedome.csh
#
# If questcl and the dome daemon are running, just write CLOSE to the
# dome daemon command file (COMMAND_FILE)
#
# If only questctl is running, send the CLOSEDOME_SIGNAL to questctl
#
# If neither questctl nor domedaemon is running, issue direct telescope_stow
# command
#
# DLR 2009 Aug 10
#
set COMMAND_FILE = "/home/observer/status_srv/dome_daemon.command"
set PID_FILE = "/home/observer/logs/questctl.pid"
set VERBOSE = 0
set CLOSEDOME_SIGNAL = 10
alias closedome_direct  '/home/observer/bin/closedome'

set questctl_on = 0
set dome_daemon_on = 0

# check if questctl is running
if ( -e $PID_FILE ) then
   set questctl_pid = `cat $PID_FILE`
   alias check_questctl 'ps -aef | grep -e " `cat $PID_FILE` " | grep -ve "grep" | wc -l'
   if ( `check_questctl` ) then
      set questctl_on = 1
   endif
endif

# check if dome_daemon is running. If not, return with warning
set l = `ps -aef | grep dome_daemon.csh | grep -ve "grep" | wc -l`
if ( $l == 1 ) then
   set dome_daemon_on = 1
endif

set t_prev = 0
if ( -e $COMMAND_FILE ) then
  set l = `cat $COMMAND_FILE | tail -n 1`
  if ( $#l == 8 ) then
     set t_prev = $l[8]
  endif
endif

echo "CLOSE " `date -u ` " " `date +"%s"` >! $COMMAND_FILE

if ( ! $questctl_on ) then
   echo "WARNING: questctl not running. Sending direct commands to TCS"
#   /home/observer/bin/closedome |& tail -n 1
   /home/observer/bin/stow_telescope |& tail -n 1
   echo "done"
else 
   if ( ! $dome_daemon_on ) then
     echo "WARNING: dome_daemon not running. Sending close signal to questctl "
     kill -$CLOSEDOME_SIGNAL $questctl_pid
   endif
   echo "dome close sequence initiated. Please wait 10 minutes ..."
endif

