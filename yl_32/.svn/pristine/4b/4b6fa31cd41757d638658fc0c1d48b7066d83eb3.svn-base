#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

PRODUCT_DIR=${DIR}/../sourcecode/product
CONFIG_DIR=${DIR}/../proj.win/run
SCENE=${DIR}/../scene
SCRIPT=${DIR}/../script
SETTING=${DIR}/../../setting

function initlog()
{
	if [ -d log ]; then
		echo "log dir exist"
		return
	fi
	mkdir log
	#cp ${PRODUCT_DIR}/logserverd.elf log/log
	ln -sf ${PRODUCT_DIR}/logserverd.elf log/log
	ln -sf ${CONFIG_DIR}/log/logserver.ini log/
}

function initccr()
{
	if [ -d ccr ]; then
		echo "ccr dir exist"
		return
	fi
	mkdir ccr
	#cp ${PRODUCT_DIR}/coordinatord.elf ccr/ccr
	ln -sf ${PRODUCT_DIR}/coordinatord.elf ccr/ccr
	ln -sf ${CONFIG_DIR}/ccr/coordinator_cfg.ini ccr/
	ln -sf ${SCENE} ccr/
	ln -sf ${SCRIPT} ccr/
	ln -sf ${SETTING} ccr/
}

function initcr()
{
	if [ -d cr ]; then
		echo "cc dir exist"
		return
	fi
	mkdir cr
	#cp ${PRODUCT_DIR}/coordinatord.elf cr/cr
	ln -sf ${PRODUCT_DIR}/coordinatord.elf cr/cr
	ln -sf ${CONFIG_DIR}/cr/coordinator_cfg.ini cr/
	ln -sf ${SCENE} cr/
	ln -sf ${SCRIPT} cr/
	ln -sf ${SETTING} cr/
}

function initgame()
{
	local gsno=$1
	if [ -d ${gsno} ]; then
		echo "${gsno} dir exist"
		return
	fi
	mkdir ${gsno}
	#cp ${PRODUCT_DIR}/gameserverd.elf ${gsno}/${gsno}
	ln -sf ${PRODUCT_DIR}/gameserverd.elf ${gsno}/${gsno}
	ln -sf ${CONFIG_DIR}/${gsno}/servercfg.ini ${gsno}/
	ln -sf ${SCENE} ${gsno}/
	ln -sf ${SCRIPT} ${gsno}/
	ln -sf ${SETTING} ${gsno}/
}

function initgate()
{
	if [ -d gate ]; then
		echo "gate dir exist"
		return
	fi
	mkdir gate
	#cp ${PRODUCT_DIR}/gatewayd.elf gate/gate
	ln -sf ${PRODUCT_DIR}/gatewayd.elf gate/gate
	ln -sf ${CONFIG_DIR}/gate/gateway_cfg.ini gate/
}

initlog
initccr
initcr

initgame gs1
initgame gs2
initgame gs3
initgame gs4
initgame cgs1
initgame cgs2

initgate
