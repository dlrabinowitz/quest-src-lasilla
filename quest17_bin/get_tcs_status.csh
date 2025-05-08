#!/bin/tcsh
#
# get_tcs_status.csh
#
# Otherwise, run domestatus and then write the TCS_FILE
#
#
echo "NOPNOP" | /home/observer/bin/tcs_talk_client.pl | cut -c 1-125
