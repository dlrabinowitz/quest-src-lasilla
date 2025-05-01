#!/bin/tcsh
#
# summarize_qa_log.csh
#
#  use bin program to get mean and rms values of mag_limit and seeing from
#  qa log
#
if ( $#argv != 1 ) then
   echo "syntax: summarize_qa_log.csh log_name"
   exit
endif
#
set TEMP_FILE = summarize_qa_log.tmp
set TEMP_FILE1 = summarize_qa_log.tmp1
set TEMP_FILE2 = summarize_qa_log.tmp2
set BIN_PATH = /usr/yaleuser1/daver/bin
set SCRIPT_PATH = /group/astro/prj/daver/quest.dir/code.dir/status_srv
set parameter = `echo "Rlim1 Rlim2 Zpoint N_det N_USNO FWHM"`
set col = `echo "1 2 3 4 5 6"`
set par_low = `echo "15 15 -5 0 0 1"`
set par_high = `echo "25 25 5 10000 10000 10"`
set par_incr = `echo "0.25 0.25 0.1 100 100 0.1"`
#

if ( $#parameter != $#col || $#parameter != $#par_low || $#parameter != $#par_high ) then
  echo "parameters list mismatch"
  exit
endif


set file = $argv[1]
if ( ! -e $file ) then
    echo "can't find file $file"
    exit
endif
#
set n_total = `cat $file |  wc -l`
set n_fail = `grep -e "-99.0 " $file | wc -l`
grep -e " s " $file | grep -e " 60.0 "  | grep -ve "-99.0 " >! $TEMP_FILE1
grep -e " s " $file | grep -e " 180.0 "  | grep -ve "-99.0 " >! $TEMP_FILE2
set n1 = `cat $TEMP_FILE1 | wc -l`
set n2 = `cat $TEMP_FILE2 | wc -l`
#
#
echo " "
echo "Number Processed : $n_total"
echo "Number Failed to Process : $n_fail"
echo " "
if ( $n1 > 0 ) then
  printf "60-sec Observations :\n"
#
  set i = 0
  foreach p ( `echo $parameter` )
    @ i = $i + 1
    set c = $col[$i]
#
#   get median value for parameter $p in column $c
#
    $SCRIPT_PATH/sort.csh $c $TEMP_FILE1 $TEMP_FILE 
    @ n = $n1 / 2
    set l = `head -n $n $TEMP_FILE | tail -n 1`
    if ( $#l >= $c ) then  
      set median = $l[$c]
    else
      set median = 0
    endif
#
#   get mean and rms 
#
    set l = `$BIN_PATH/bin $c $par_low[$i] $par_high[$i] $par_incr[$i] $TEMP_FILE1 | grep -e "sigma"`
    if ( $#l >= 7 ) then
      set mean = $l[4]
      set rms = $l[7]
    else
      set mean = 0.0  
      set rms = 0.0   
    endif
#
#   print values
#
    if ( $i <= 3 || $i == 6 ) then 
        printf "%10s : median %5.1f  mean %5.1f  rms %5.1f\n" $p $median $mean $rms
    else
        printf "%10s : median %5.0f  mean %5.0f  rms %5.0f\n" $p $median $mean $rms
    endif
  end
  printf "\n"
endif
#
if ( $n2 > 0 ) then
  printf "180-sec Observations :\n"
#
  set i = 0
  foreach p ( `echo $parameter` )
    @ i = $i + 1
    set c = $col[$i]
#
#   get median value for parameter $p in column $c
#
    $SCRIPT_PATH/sort.csh $c $TEMP_FILE2 $TEMP_FILE 
    @ n = $n2 / 2
    set l = `head -n $n $TEMP_FILE | tail -n 1`
    if ( $#l >= $c ) then  
      set median = $l[$c]
    else
      set median = 0
    endif
#
#   get mean and rms 
#
    set l = `$BIN_PATH/bin $c $par_low[$i] $par_high[$i] $par_incr[$i] $TEMP_FILE2 | grep -e "sigma"`
    if ( $#l >= 7 ) then
      set mean = $l[4]
      set rms = $l[7]
    else
      set mean = 0.0  
      set rms = 0.0   
    endif
#
#   print values
#
    if ( $i <= 3 || $i == 6 ) then 
        printf "%10s : median %5.1f  mean %5.1f  rms %5.1f\n" $p $median $mean $rms
    else
        printf "%10s : median %5.0f  mean %5.0f  rms %5.0f\n" $p $median $mean $rms
    endif
  end
  printf "\n"
endif
#
