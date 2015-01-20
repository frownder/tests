FILE_COUNT=0
DIR_COUNT=0

for FILE in * 
do
    if [ -f $FILE ];then
        ((FILE_COUNT++))   #let FC=FC+1
    elif [ -d $FILE ];then
        ((DIR_COUNT++))
    fi
done
echo "F D Count" $FILE_COUNT $DIR_COUNT

#simple Using Filter
echo "Simple F count: $(ls -l | grep ^- | wc -l)"
