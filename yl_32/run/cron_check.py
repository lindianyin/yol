#!/usr/bin/python
# -*- coding: utf-8 -*-
import requests
import json
import commands
import time
import os
import sys 

while True:
	(status, output) = commands.getstatusoutput('bash dashenctl.sh check')
	time.sleep(1)
	print(time.strftime('%Y-%m-%d %H:%M:%S'), status, output)
	sys.stdout.flush()
