fcount=0
dcount=0
for FILE in `ls /tmp/*`
do
if [ -d $FILE ] ; then
let dcount=dcount+1
echo " $FILE is dir"
else 
let fcount=fcount+1
echo " $FILE is file"
fi
done
echo " Dir count : $dcount"
echo " File count : $fcount"

