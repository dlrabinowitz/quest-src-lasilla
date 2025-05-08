#!/bin/tcsh
#
# dome_init.csh
#
# send command to make current dome position the stow position
#
echo "DOMEINIT" | ~/bin/tcs_talk_client.pl
#
