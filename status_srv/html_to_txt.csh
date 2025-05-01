#!/bin/tcsh
#
# html_to_txt.csh
#
# strip out "<br>"
#
set file = $argv[1]
set n = `cat $file | wc  -l`
set i = 1
while ( $i <= $n )
  set l = `head -n $i $file | tail -n 1`
  set p = `echo $l | grep -e "<br>" | wc -l`
  set q = `echo $l | grep -e "html>" | wc -l`
  if ( $p == 0  && $q == 0 ) echo $l
  @ i = $i + 1
end

