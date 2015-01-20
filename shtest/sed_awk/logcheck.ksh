#!/bin/ksh
# name: logcheck.ksh
# written by: aini(youngpia@lycos.co.kr)
# desc: check /var/adm/messages for user facility log messages
# args: none (pattern and logfile may be args...)

pattern="\<user\."
outfile=userlog.$$
logfile=/var/adm/messages

# I'll ignore HUP. So, even if you logout, I'll continue...
trap ' ' 1
#trap '[ -f $outfile ] && rm $outfile ; exit' 2 3 9 15
trap '[ -a $outfile ] && rm $outfile ; exit' 0 

#if need, check already running...

# open log file for input
exec < $logfile

if [ -a $outfile ]; then	# maybe a directory? oops...
	cat /dev/null > $outfile
else
	touch $outfile
fi

# go to the end of the file(EOF)
while read line
do
	:
done

# loop forever. user can terminate me by pressing Ctrl-C
while true
do
	# I've reached the end of the file.
	# Now, I'll sleep for 60 seconds for log messages to be added.
	sleep 60

	while read line
	do
		echo $line | grep "$pattern" >> $outfile 2> /dev/null
	done

	if [ -s $outfile ]; then	# log pattern found
		chktime=$(date +%D" "%T)

		# make mail contents(header and footer).
		# Better to make the header a variable.
		nawk -v file=$logfile -v checktime="$chktime" \
		       'BEGIN {print "\n"} \
			BEGIN {print "-------------------------------------------------"} \
			BEGIN {print "user.error log check from " file}; \
			BEGIN {print "checked at " checktime}; \
			BEGIN {print "-------------------------------------------------"} \
			{print}; \
			END {print "-------------------------------------------------"} \
			END {print "Total number of messages: " NR} \
			END {print "-------------------------------------------------"}' \
			$outfile | cat - > $outfile
			 
		mail root < $outfile 

		cat /dev/null > $outfile	# make result file zero-sized
	fi

done
