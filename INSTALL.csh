mkdir /home/observer
mkdir /home/observer/logs
mkdir /home/observer/bin
install quest-src-lasilla at /home/observer
cd quest-src-lasilla
cd lib
make clean
make all
cd ../prog
make clean
make all
make install
cd ../util
cp *csh /home/observer/bin
# these perl files are probably obsolete
cp *pl /home/observer/bin
#

