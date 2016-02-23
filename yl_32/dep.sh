#!/bin/bash
rm -rf ../server/gameserver/gameserverd.elf
rm -rf ../server/coordinator/coordinatord.elf
rm -rf ../server/logserver/logserverd.elf
rm -rf ../server/gateway/gatewayd.elf
rm -rf /usr/lib/libcoreD.so
#rm -rf ../script
#rm -rf ../setting


cp ./gameserverd.elf ../server/gameserver/
cp ./coordinatord.elf ../server/coordinator/
cp ./logserverd.elf ../server/logserver/
cp ./gatewayd.elf  ../server/gateway/
cp ./libcoreD.so   /usr/lib/libcoreD.so
#cp -r ./script  ../
#cp -r ./setting ../

ldconfig
