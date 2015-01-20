#!/bin/ksh

integer count=0

while (( count < 1000 ))
do
	(( count += 1 ))
	logger -p user.error "log message from user.error for logcheck test: $count"
	sleep 5 
done
