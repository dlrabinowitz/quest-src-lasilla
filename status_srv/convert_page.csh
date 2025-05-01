#!/bin/tcsh
#
# convert_page.csh [web_page] [ history_file]
#
# read the quest_monitor.html page, pull out the numerical status values,
# and print them out on one line
#
if ( $#argv != 2 ) then
   echo "syntax: convert_page.csh [web_page] [ history_file]"
   exit
endif
#
set dome_status = -2
set camera_status = -2
#
set WEB_PAGE = $argv[1]
set HISTORY_FILE = $argv[2]
if ( ! -e $WEB_PAGE ) then
   echo "can't find web page at $WEB_PAGE"
   exit
endif
#
if ( ! -e $HISTORY_FILE ) then
   echo "can't find history file at $HISTORY_FILE"
   exit
endif
#
set d = `date -u +"%Y%m%d%H%M%S"`
set d1 = `date -u +"%Y %m %d %H %M %S"`
set l = `get_almanac.csh $d1 | grep -e "Date"`
set jd = `echo "scale=6; $l[5] - 2450000.0" | bc`
set line = "$d $jd"
set n = `cat $WEB_PAGE | wc -l`
set i = 1

#
# keep reading next line until "dome status" word is found
set done = 0
while ( $done ==  0 && $i < $n )
  set l = `head -n $i $WEB_PAGE | tail -n 1`
  set m = `echo $l | grep -e "dome status" | wc -l`
  if ( $m > 0 ) then
     set done = 1
  endif
  @ i = $i + 1
end
#
# get the dome status from the last line read
set m1 = `echo $l | grep -e "CLOSED" | wc -l`
set m2 = `echo $l | grep -e "OPEN" | wc -l`
if ( $m1 == 1 ) then
   set dome_status = 0
endif
if ( $m2 == 1 ) then
   set dome_status = 1
endif
if ( $m1 == 0 && $m2 == 0 ) then
   set dome_status = -1
endif
#
# keep reading next line until "camera status" word is found
set done = 0
while ( $done ==  0 && $i < $n )
  set l = `head -n $i $WEB_PAGE | tail -n 1`
  set m = `echo $l | grep -e "camera status" | wc -l`
  if ( $m > 0 ) then
     set done = 1
  endif
  @ i = $i + 1
end
#
# get the camera status from the last line read
set m1 = `echo $l | grep -e "OFF" | wc -l`
set m2 = `echo $l | grep -e "ON" | wc -l`
if ( $m1 == 1 ) then
   set camera_status = 0
endif
if ( $m2 == 1 ) then
   set camera_status = 1
endif
if ( $m1 == 0 && $m2 == 0 ) then
   set camera_status = -1
endif
#
set line = "$line $dome_status $camera_status " 
#
# keep reading next line until "Telescope" word is found
set done = 0
while ( $done ==  0 && $i < $n )
  set l = `head -n $i $WEB_PAGE | tail -n 1`
  set m = `echo $l | grep -e "Telescope" | wc -l`
  if ( $m > 0 ) then
     set done = 1
  endif
  @ i = $i + 1
end


# now keep reading each line. Whenever at ":" is found, skip
# three lines, read the value in variable l, and add it to
# the line string
while ( $i <= $n )
   set l = `head -n $i $WEB_PAGE | tail -n 1`
   set m = `echo $l | grep -e ":" | wc -l`
   if ( $m > 0 ) then
       @ i = $i + 3
       set l = `head -n $i $WEB_PAGE | tail -n 1`
       set line = "$line `printf "%s" $l`"
   endif
   @ i = $i + 1
end
#
# print the line string to the history file
echo $line >> $HISTORY_FILE
chmod 777 $HISTORY_FILE
