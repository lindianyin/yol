#!/usr/bin/python
# -*- coding: utf-8 -*-
import requests
import json
import commands
import time
import os
import sys

def getToken(corpid, corpsecret):
	r = requests.get('https://qyapi.weixin.qq.com/cgi-bin/gettoken?corpid=' + corpid + '&corpsecret=' + corpsecret)
	return json.loads(r.text)['access_token']

def sendMsg(token, text):
	headers = { "encoding:": "utf-8"}
	payload = {
		"touser": "@all",
		"msgtype": "text",
		"agentid": 0,
		"text": {
			"content": text
		},
		"safe":"0"
	}
	r = requests.post('https://qyapi.weixin.qq.com/cgi-bin/message/send?access_token=' + token + "&debug=1", data = json.dumps(payload, ensure_ascii=False), headers = headers)

try:
	#argv[1]为文件名，将文件内容作为msg发送给微信客户端,msg文件必须为utf-8 withtout BOM格式
	msgfile=sys.argv[1]
	f = open(msgfile, "r")
	msg = f.read()
except Exception,ex:
	print Exception,":",ex
	sys.exit(0)

corpid='wxe059fc18a2cd4a62'
corpsecret='041rwfoYpG0CbQEfVtjIQQDxu7ouKaa3jFjOTN3ltj5aKv7Bqo9fjOFuDqOCVEx_'
token = getToken(corpid, corpsecret)
sendMsg(token, msg)
