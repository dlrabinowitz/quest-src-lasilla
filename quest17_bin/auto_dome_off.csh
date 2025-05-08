#!/bin/tcsh
#
# auto_dome_on.csh
#
# send command to stop the dome from automatically following the telescope position
#
echo "AUTODOME OFF" | ~/bin/tcs_talk_client.pl
#
