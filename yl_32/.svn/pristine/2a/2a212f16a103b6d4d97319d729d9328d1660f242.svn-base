#!/bin/sh
path=$(cd "$(dirname "$0")"; pwd)
fname=`date +%Y%m%d_%H_%M_%S`
echo $fname
echo $path
mv $path/2015* $path/bak
mkdir $path/$fname
echo $path
cd $path/LibQrt/Src/LogServer
echo $path/LibQrt/Src/LogServer
#make clean
make Debug
echo "**************************LogServer is over!!****************************"
sleep 2


cd $path/LibQrt
#make clean
make Debug
echo "**************************LibQrt is over!!******************************"
sleep 2



cd $path/sourcecode
make clean
make debug
echo "*************************servers is over!!******************************"

echo "*************************deploy the new server!!******************************"

sleep 10


cp $path/LibQrt/Src/Core/Debug/libcoreD.so $path/$fname
cp $path/sourcecode/product/debug/coordinator/coordinatord.elf $path/$fname
cp $path/sourcecode/product/debug/gameserver/gameserverd.elf $path/$fname
cp $path/sourcecode/product/debug/gateway/gatewayd.elf $path/$fname
cp $path/LibQrt/Src/LogServer/Debug/logserverd.elf $path/$fname
cp $path/dep.sh $path/$fname
#cp -r $path/script $path/$fname
#cp -r $path/setting $path/$fname

cd $path
tar -zcvf $fname.tar.gz $fname
#mv $path/$fname $path/bak
#exit 0

service dashen stop

mv  $path/$fname /data/dashen/
cd /data/dashen/$fname
/data/dashen/$fname/dep.sh
service dashen start
