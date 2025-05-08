#!/bin/tcsh
#
# send TCS command to tcs_talk_client.pl
#
set line = $<
echo $line | /home/observer/bin/tcs_talk_client.pl

