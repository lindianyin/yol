#!/bin/bash

#-----------------------------------------------#

#This is a  free GNU GPL version 3.0 or abover

#Copyright (C) 2008 06 05

#mysql_backup Dedicated copyright by My

#-----------------------------------------------#

echo -e [`date +"%Y-%m-%d %H:%M:%S"`] start

#system time

time=`date +"%y-%m-%d"`

#host IP

host="127.0.0.1"

#database backup user

user="backup"

#database password

passwd="123456"

#Create a backup directory

mkdir -p /data/backup/db/"$time"

#list database name

all_database=`/usr/bin/mysql -u$user -p$passwd  -Bse 'show databases'`

#in the table from the database backup

for i in $all_database
do
[ "$i" != "information_schema" -a "$i" != "mysql" ] && /usr/bin/mysqldump -u$user -p$passwd $i > /data/backup/db/"$time"/"$i"_"$time".sql
done

echo -e [`date +"%Y-%m-%d %H:%M:%S"`]  end

exit 0
