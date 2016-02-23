#!/bin/bash
cd "$( dirname "${BASH_SOURCE[0]}" )"

ulimit -c unlimited

#检查指定pid进程是否还存活,返回1表示存活
function checkpid()
{
	local pid=$1
	local lines=`ps -p ${pid} | wc -l`
	if [ ${lines} -eq 2 ]; then
		return 1
	fi
	return 0
}

function stopbypidfile()
{
	local pidfile=$1
	local pid=`cat ${pidfile}`
	checkpid ${pid}
	if [ $? -eq 1 ]; then
		kill -9 ${pid}
		echo "kill -9 ${pid}"
	else
		echo "Warning:${pidfile}(${pid}) 进程早已退出!!!"
	fi
	mv ${pidfile} ${pidfile}.old
}

function start()
{
	local dir=$1
	local program=$1
	cd ${dir}
	./${program} &
	echo $! >${program}.pid
	cd -
}

function stop()
{
	local dir=$1
	local program=$1
	local pidfile=${dir}/${program}.pid
	if [ -e ${pidfile} ]; then
		stopbypidfile ${pidfile}
	else
		echo "Warning: ${program} 未启动"
	fi
}


function check()
{
	local dir=$1
	local program=$1
	local pidfile=${dir}/${program}.pid
	if [ -e ${pidfile} ]; then
		local pid=`cat ${pidfile}`
		checkpid ${pid}
		if [ $? -eq 0 ]; then
			local IP=`curl -s www.wyown.com/clientip`
			echo "${IP}" >msg.tmp
			echo "${program}(${pid}) 进程崩溃了!!!" >> msg.tmp
			./sendmsg.py msg.tmp
			rm -fr msg.tmp
			mv ${pidfile} ${pidfile}.old
		else
			echo "${program}(${pid}) 运行正常"
		fi
	else
		echo "Warning: ${program} 未启动"
	fi
	return
}

if [ $# -ne 1 ]; then
	echo -e "usage:\n\t $0 start|stop|check"
	exit 0
fi

if [ $1 = "start" ]; then
	start log
	sleep 1
	start ccr
	sleep 3
	start cr
	sleep 2
	start gs1
	start gs2
	start gs3
	start gs4
	start cgs1
	start cgs2
	sleep 1
	start gate

	if [ -e "hook_start.sh" ]; then
		bash hook_start.sh
	fi
elif [ $1 = "stop" ]; then
	if [ -e "hook_stop.sh" ]; then
		bash hook_stop.sh
	fi

	stop gate 
	stop cgs2
	stop cgs1
	stop gs4
	stop gs3
	stop gs2
	stop gs1
	stop cr
	stop ccr
	stop log
elif [ $1 = "check" ]; then
	check log
	check ccr
	check cr
	check gs1
	check gs2
	check gs3
	check gs4
	check cgs1
	check cgs2
	check gate
fi
