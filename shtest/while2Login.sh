WAITING="root"

#who | grep $WAITING
#while (($?!=0)) 
until who | grep $WAITING > /dev/null 2>&1
do
    echo "$WAITING No login"
    sleep 3
#   who | grep $WAITING > /dev/null 2>&1
done
echo "$WAITING login"
