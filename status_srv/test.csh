#!/bin/tcsh
`date` >& test.log
alias domestatus 'echo "domestatus" | /home/observer/quest-src-lasilla/util/questclient.pl'
set i = 1
while ( $i <= 10 )
domestatus >>& test.log
sleep 3
end
