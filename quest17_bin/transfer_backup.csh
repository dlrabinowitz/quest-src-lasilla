#!/bin/tcsh
#
# transfer_backup.csh
# DLR 2010 03 05
#
# make a backup of untransferred QUEST data to the USB Terabyte disk
# only backup data less than MAX_AGE seconds old.
#
#
unalias cp
unalias rm
set BACKUP_DIR = "/scr3/quest/data"
set TRANSFER_DIR = "/scr1/quest/data/quest/data/realtime/transfer_backup"
# set MAX_AGE to 1 day in seconds
set MAX_AGE = 86400
#
if ( ! -e $BACKUP_DIR ) then
  echo "ERROR: can't find BACKUP_DIR = $BACKUP_DIR "
  exit
endif
#
if ( ! -e $TRANSFER_DIR ) then
  echo "ERROR: can't find TRANSFER_DIR = $TRANSFER_DIR"
  exit
endif
 
set d = `date +"%Y%m%d"`
set dest_dir = "$BACKUP_DIR/$d"
set TEMP_FILE = "$TRANSFER_DIR/transfer_backup.$d"
#
if ( ! -e $dest_dir ) then
   mkdir $dest_dir
   chmod -R 777 $dest_dir
endif
set LOG_FILE = "$dest_dir/transfer_backup.log"
#
cd $TRANSFER_DIR
echo `date` " Starting transfer backup" >! $LOG_FILE
#
foreach file ( `ls` )
      echo `date` " Copying $file to $dest_dir/$file" >> $LOG_FILE
      cp $file $dest_dir
      chmod 777 $dest_dir/$file
      echo `date` " Removing $file from $TRANSFER_DIR" >> $LOG_FILE
      rm $TRANSFER_DIR/$file
end
echo `date` "Done with transfer backup" >> $LOG_FILE
