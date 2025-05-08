#!/bin/tcsh
#
# move_dome.csh
#
# send command to move the dome to a specified position (0 for due north,
# 180 for south, range 0 to 359.99
#
if ( $#argv != 1 ) then
   echo "syntax: move_dome.csh [deg]"
   exit
endif
echo "MOVDOME $argv[1]" | ~/bin/tcs_talk_client.pl
#
