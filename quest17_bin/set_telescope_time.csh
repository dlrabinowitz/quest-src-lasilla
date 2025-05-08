#!/bin/tcsh
#
# script to sync time of TCS computer with local computer time
#
#
# get the UT date from the system clock
#
set d = `date -u +"%H %M %S"`
#
# prepare the command to go to the TCS
#
set line = `printf "SETTIME %02d%02d%04.1f" $d[1] $d[2] $d[3]`
echo $line
echo
#
# send the command
#
echo $line | ~/quest-src-lasilla/tcs_talk/tcs_talk
echo "NOPNOP" | ~/quest-src-lasilla/tcs_talk/tcs_talk
