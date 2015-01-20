#!/bin/ksh
PS3="Eneter the number for your color choice:"
select color in Red Yellow Orange Green Blue Purple Darkblue Exit 
do
case $color in
	   Red) echo "This is Red" ;;
        Yellow) echo "This is Yellow";;
        Orange) echo "This is Orange";;
        Green) echo "This is Green";;
        Blue) echo "This is Blue";;
        Purple) echo "This is Purple";;
        Darkblue) echo "This is Dark blue";;
        Exit) exit;;
        *) echo "Select Red,Yellow,Prange,Green,Blue,Purple,Darkblue";;
esac
done
