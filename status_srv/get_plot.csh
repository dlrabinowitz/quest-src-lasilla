#!/bin/tcsh
#
# get_plot.csh
#
# get plot of temperature and pressure from La Silla using wget
#
cd ~/web/sedna/status
#
#
wget --user=tios --password=CASAperro -O util_log.pdf http://134.171.80.151/util_log.pdf  >& /tmp/get_plot.tmp
#
