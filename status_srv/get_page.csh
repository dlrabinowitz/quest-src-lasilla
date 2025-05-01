#!/bin/tcsh
##
# get_page.csh
#
# periodically use wget to copy quest_monitor.html webpage at 134.171.80.151 
# to publically available address at www.yale.edu/quest/sedna/status.
#
# NOTE: This webpage auto updates, so the html code must be altered to
# give the new address as the address to auto update
#
# Also notify recepients by email if the camera status changes from
# on to off or visa-versa
#
# DLR 2009 Sep 1
#
set SCRIPT_PATH = "/group/astro/prj/daver/quest.dir/code.dir/status_srv"
set STATUS_DIRECTORY = "/group/astro/prj/web/sedna/status"
set HISTORY_FILE = "$STATUS_DIRECTORY/history.dat"
set WEB_PAGE = "$STATUS_DIRECTORY/quest_monitor.html"
set QA_LOG = $STATUS_DIRECTORY/qa1.log
#
cd $STATUS_DIRECTORY 
set previous_camera_state = "ON"
#
set recipients = "david.rabinowitz@yale.edu, nancy.ellman@yale.edu"
#
#
set i = 5
set previous_camera_state = "ON"
while ( 1 )
  wget --user=tios --password=CASAperro -O temp.html http://134.171.80.151/auto_update/quest_monitor.html >& /tmp/get_page.tmp
  cat temp.html | sed -e 's/remote-quest.ls.eso.org\/auto_update/www.yale.edu\/quest\/sedna\/status/' > temp1.html
  cat temp1.html | sed -e 's/CONTENT=\"10/CONTENT=\"60/' > temp.html
  cat temp.html | sed -e 's/Camera Status/<a href=util_log.jpg>Camera Status<\/a>/' > temp1.html
  cat temp1.html | sed -e 's/Telescope Status/<a href=telstat.jpg>Telescope Status<\/a>/' > $WEB_PAGE
#
  set l = `grep -e "camera status" $WEB_PAGE | grep -e "OFF" | wc -l`
  if ( $l == 1 ) then
    if ( $previous_camera_state != "OFF" ) then
         echo "camera power out" | mail -s "QUEST CAMERA POWER OUT" $recipients 
         set previous_camera_state = "OFF"
    endif
  else
    if ( $previous_camera_state != "ON" ) then
         echo "camera power back on" | mail -s "QUEST CAMERA POWER BACK ON"  $recipients
         set previous_camera_state = "ON"
    endif
  endif
  wget --user=tios --password=CASAperro -O temp.html http://134.171.80.151/auto_update/quest_webcam.html >& /tmp/get_page.tmp
  cat temp.html | sed -e 's/remote-quest.ls.eso.org\/auto_update/www.yale.edu\/quest\/sedna\/status/' > temp1.html
  cat temp1.html | sed -e 's/CONTENT=\"10/CONTENT=\"60/' > quest_webcam.html
  wget --user=tios --password=CASAperro -O webcam_image.jpg http://134.171.80.151/auto_update/webcam_image.jpg >& /tmp/get_page.tmp
#
#   get temperature and pressure log and obs log
#
  @ i = $i + 1
  if ( $i >= 5 ) then
    wget --user=tios --password=CASAperro -O util_log.pdf http://134.171.80.151/util_log.pdf  >& /tmp/get_page.tmp
    convert util_log.pdf util_log.jpg
    wget --user=tios --password=CASAperro -O log.obs http://134.171.80.151/log.obs  >& /tmp/get_page.tmp
    wget --user=tios --password=CASAperro -O qa.log http://134.171.80.151/qa.log   >& /tmp/get_page.tmp
    set i = 1
  endif
#
# strip out LIGO observations from log.obs
  if ( -e log.obs ) then
     grep -e "LIGO" log.obs > log.ligo
     chmod 777 log.ligo
  endif
#
# strip LIGO observations from qa.log
  if ( -e $QA_LOG ) then
     grep -e "#starting" $QA_LOG >! qa.ligo
     grep -e "#QA Chip" $QA_LOG >> qa.ligo
     grep -e "#Rlim" $QA_LOG >> qa.ligo
     grep -e "LIGO" $QA_LOG > qa.ligo
     chmod 777 qa.ligo
  endif
#
# add to history
  $SCRIPT_PATH/convert_page.csh $WEB_PAGE $HISTORY_FILE
  $SCRIPT_PATH/make_telstat_macro.csh history.dat > telstat.macro
  sm < telstat.macro >& /tmp/sm.out
  convert telstat.ps telstat.jpg
#
  sleep 60
end
