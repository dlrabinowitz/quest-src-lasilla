#!/bin/tcsh
#
# dome_status.csh
#
# Determine if dome shutter is opened, closed, or neither
# If questctl is running, read TCS file to determine status.
# Otherwise, use domestatus command.
#
#
# DLR 2009 Aug 10
#
set VERBOSE = 0


set temp_file = "/tmp/dome_status.tmp"


# see if schmidt dome is open
#
#
  if ($VERBOSE == 1 ) then
    echo `date` " checking dome status" >> $dome_log
  endif

  domestatus >& $temp_file
  set l2 = `cat $temp_file | tail -n 1 | grep -e "dome open" | wc -l`
  set l3 = `cat $temp_file | tail -n 1 | grep -e "failed" | wc -l`
  set l1 = 0
  @ l1 = $l2 - $l3

  if ( $l1 == 1 ) then
     echo "open"
  else if ( $l1 == 0 ) then
     echo "closed"
  else
     echo "unknown"
  endif
#
