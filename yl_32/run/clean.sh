#!/bin/bash
function make_backup_dir_and_echo ()
{
	local DATE=`date +%Y-%m-%d`
	local DIR_NAME=${DATE}
	local NO=1
	while [ -d "${DIR_NAME}/${NO}" ]
	do
		NO=$((NO+1))
	done
	mkdir -p ${DIR_NAME}/${NO}
	echo ${DIR_NAME}/${NO}
	
}

BACKDIR=`make_backup_dir_and_echo`
mv ccr cr cgs1 cgs2 gs1 gs2 gs3 gs4 gate log ${BACKDIR}/
