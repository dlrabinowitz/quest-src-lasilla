#!/bin/tcsh
#
# get_log.csh
set TEST = 0
#
# get daily logs from quest16 at La Silla using wget
#
set d = `date +"%Y %B %d"`
set d1 = `date +"%Y%m%d"`
#
if ( $TEST ) then
    set quest_group = "david.rabinowitz@yale.edu"
    set log_group = "david.rabinowitz@yale.edu"
else
#    set log_group = "nancy.ellman@yale.edu,david.rabinowitz@yale.edu,megan.schwamb@yale.edu,ellie4physics@yahoo.com"
    set log_group = "nancy.ellman@yale.edu,david.rabinowitz@yale.edu,megan.schwamb@yale.edu,ellie.hadjiyska@yale.edu"
    set quest_group = "david.rabinowitz@yale.edu,megan.schwamb@yale.edu,ellie.hadjiyska@yale.edu,nancy.ellman@yale.edu,charles.baltay@yale.edu,rochelle.lauer@yale.edu,william.emmet@yale.edu"
endif
#
set SAVE_DIRECTORY = /usr/yaleuser1/daver/tnos/logs/qa
#set SAVE_DIRECTORY_QA = /group/astro/tno/logs/db/log_qa
#set SAVE_DIRECTORY_LOG = /group/astro/tno/logs/db/log_obs
set STATUS_DIRECTORY = /usr/yaleuser1/daver/web/sedna/status
set QA_LOG = $STATUS_DIRECTORY/qa1.log
set QA_PLOT = $STATUS_DIRECTORY/qa1.jpg
set LOG_OBS = $STATUS_DIRECTORY/log.obs
set SCRIPT_PATH = "/group/astro/prj/daver/quest.dir/code.dir/status_srv"
#
if ( ! -e $STATUS_DIRECTORY ) then
   echo "ERROR: can't find status directory $STATUS_DIRECTORY"
   exit
endif
cd $STATUS_DIRECTORY  
#
# get daily check results
#
# first erase previous report
#
echo "No Report" >! /tmp/get_log.tmp
#
# Now get latest system check from quest16 web server
#
wget --user=tios --password=CASAperro -O lasilla_quest_log.html http://134.171.80.151/lasilla_quest_log.html  >& /tmp/get_log.tmp
html_to_txt.csh  lasilla_quest_log.html > /tmp/get_log.tmp
#
# mail daily check to log_group
mail  -s "La Silla Quest Daily Check $d" $log_group < /tmp/get_log.tmp
##
#
# get daily observation report
#
# first erase previous report
#
echo "No Log" >! /tmp/get_log.tmp
#
wget --user=tios --password=CASAperro -O lasilla_quest_report.html http://134.171.80.151/lasilla_quest_report.html  >& /tmp/get_log.tmp
#
cat lasilla_quest_report.html | grep -ve "html" | head -n 1>! /tmp/get_log.tmp
echo "" >> /tmp/get_log.tmp
#
if ( -e $QA_LOG ) then
  $SCRIPT_PATH/summarize_qa_log.csh $QA_LOG >> /tmp/get_log.tmp
else
  echo "" >> /tmp/get_log.tmp
endif
#
cat lasilla_quest_report.html | grep -ve "html" | grep -ve "Report" >> /tmp/get_log.tmp
#
# mail to quest_group
#
mail  -s "La Silla Quest Observations: $d" $quest_group < /tmp/get_log.tmp

#
# make a copy of the report
if ( -e /tmp/get_log.tmp ) then
  cp /tmp/get_log.tmp $SAVE_DIRECTORY/report_$d1
endif
#

# get the qa log and mail a copy, also save a copy
if ( -e $QA_LOG ) then
#  cp $QA_LOG $SAVE_DIRECTORY_QA/qa_"$d1".log
#  /group/astro/prj/daver/quest.dir/code.dir/status_srv/dirmail -s "La Silla QA Log : $d" -t $quest_group -f david.rabinowitz@yale.edu $SAVE_DIRECTORY/qa_"$d1".txt >& /tmp/get_log.tmp

endif
#
# get qa plot, mail a copy, and save a copy
if ( -e $QA_PLOT ) then
  cp $QA_PLOT $SAVE_DIRECTORY/qa_"$d1".jpg
  /group/astro/prj/daver/quest.dir/code.dir/status_srv/dirmail -s "La Silla QA plot : $d" -t $quest_group -f david.rabinowitz@yale.edu $SAVE_DIRECTORY/qa_"$d1".jpg >& /tmp/get_log.tmp
endif
#
# get log.obs, and save a copy
if ( -e $LOG_OBS ) then
#  cp $LOG_OBS $SAVE_DIRECTORY_LOG/log.obs."$d1"
endif
#
#
if ( 0 ) then
# get weather monitor plot    
#
# first erase previous report
#
#
set report_name = end$d1.ps
wget  -O $report_name http://www.ls.eso.org/lasilla/dimm/Archive/$report_name >& /tmp/get_log.tmp
#
# convert report to pdf
if ( -e $report_name) then
  ps2pdf $report_name
  if ( -e end$d1.pdf ) then
     rm $report_name
     set report_name = end$d1.pdf
  endif
endif
#
# mail to quest_group
#
if ( -e $report_name ) then
#  /group/astro/prj/daver/quest.dir/code.dir/status_srv/dirmail -s "La Silla Weather Log : $d1" -t $quest_group -f david.rabinowitz@yale.edu $report_name >& /tmp/get_log.tmp
#
# make a copy
  cp $report_name $SAVE_DIRECTORY
# remove it
  rm $report_name
endif
endif
#
