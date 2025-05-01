#!/bin/tcsh
cd ~/web/sedna/status
while ( 1 )
  wget --user=tios --password=CASAperro -O temp.html http://134.171.80.151/auto_update/quest_monitor.html >& /tmp/get_page.tmp
  cat temp.html | sed -e 's/134.171.80.151\/auto_update/www.yale.edu\/quest\/sedna\/status/' > temp1.html
  cat temp1.html | sed -e 's/CONTENT=\"10/CONTENT=\"60/' > quest_monitor.html
  wget --user=tios --password=CASAperro -O temp.html http://134.171.80.151/auto_update/quest_webcam.html >& /tmp/get_page.tmp
  cat temp.html | sed -e 's/134.171.80.151\/auto_update/www.yale.edu\/quest\/sedna\/status/' > temp1.html
  cat temp1.html | sed -e 's/CONTENT=\"10/CONTENT=\"60/' > quest_webcam.html
  wget --user=tios --password=CASAperro -O webcam_image.jpg http://134.171.80.151/auto_update/webcam_image.jpg >& /tmp/get_page.tmp
  sleep 60
end
