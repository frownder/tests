#!/bin/ksh
typeset -u NEWFILE
for file in *
do

    if (($file != *[A-Z]*));then
        continue
    fi
ORIGIN=$file
NEWFILE=$file
mv $ORIGIN $NEWFILE 
echo "mv $ORIGIN $NEWFILE"

done
