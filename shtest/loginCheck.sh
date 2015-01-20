if [ $# == 0 ];then
#   echo "Usage: command userID"
#   exit 1
    echo -n "input userID:"
    read USERID
else
    USERID=$1
fi

who | grep $USERID > /dev/null 2>&1

if [ $? == 0 ];then
    echo $USERID "logged in"
else
    echo $USERID "NOT logged in"
fi
