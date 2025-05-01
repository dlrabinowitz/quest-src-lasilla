#!/bin/tcsh
#
# start get_page.csh which grabs the quest_monitor webpage, quest_webcam webpage, 
# log.obs, qa.log, etc from the web server on quest16
#
# first kill any ongoing version of get_page.csh
#
unalias mv
unalias rm
unalias cp
set HISTORY_FILE = "/group/astro/prj/web/sedna/status/history.dat"
set get_page_dir = "/group/astro/prj/daver/quest.dir/code.dir/status_srv"
set get_page = "$get_page_dir/get_page.csh"
#
if ( ! -e $get_page ) then
  echo "can't find get_page.csh at $get_page"
  exit
endif
cd $get_page_dir
#
#
set l = `ps -aef | grep get_page | grep daver | grep -ve grep | grep -ve start `
if ( $#l == 9 ) then
  echo "get_page is running"
  echo "pid is $l[2]"
  while ( $#l == 9 )
    echo "$l"
    echo "killing $l[2]"
    kill -9 $l[2]
    set l = `ps -aef | grep get_page | grep daver | grep -ve grep | grep -ve start`
  end
else
  echo "no version of get_page.csh currently running under user daver"
endif
#
echo `date` " starting get_page.csh" >! get_page.out
if ( -e $HISTORY_FILE ) then
   tail -n 2000 $HISTORY_FILE >! history.tmp
   mv history.tmp $HISTORY_FILE
else 
   echo "# `date` start of LSQ history file" >! $HISTORY_FILE
endif
echo "#YYYYMMDDHHMMSS MJD Dome Camera RA(hr)  Dec(deg)  HA(hr)   LST(hr)  JD        UT(hr)    Epoch  Az   Elev Secz Focus Dome Wobble  Hor-Lm   RA-lm       Dec-lm       Servos   Dome  RA-st Dec-st temp hum wspd wdir dpt pres 2.2m camera cam-press ccd-tmp1 ccd-tmp2 ccd-tmp3 ccd-tmp4 v1 v2 v3 v4"  >> $HISTORY_FILE
$get_page >>& get_page.out &
