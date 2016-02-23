all:lua logserver game

lua:
	cd 3rd/lua-5.1.4 && make linux

logserver:core mysqldb
	make -C LibQrt/Src/LogServer Debug

core:
	make -C LibQrt/Src/Core -f Core.mak CFG="Debug"

mysqldb:
	make -C LibQrt/Src/MysqlDB -f QMysqlDB.mak CFG="Debug"

game:
	make -C sourcecode debug

clean:
	make -C LibQrt/Src/LogServer clean
	make -C LibQrt/Src/Core -f Core.mak CFG="Debug" clean
	make -C LibQrt/Src/MysqlDB -f QMysqlDB.mak CFG="Debug" clean
	make -C sourcecode clean
	rm -fr LibQrt/Lib/* 
elfclean:
	rm -fr sourcecode/product/*
install:
	sudo cp sourcecode/product/*.elf /data/dashen/server/product/
