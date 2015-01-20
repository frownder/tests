PS3="Enter menu num:"

select MENU in red yellow orange green exit
do
case $MENU in
    red) echo "r";;
    yellow) echo "y";;
    orange) echo "o";;
    green) echo "g";;
    exit) exit 0;;
    *) echo "select red, yellow, orange, green, exit ";;
esac
done

