IFS=:
read BASES < data.txt

for B in $BASES
do
    for gugu in 1 2 3 4
    do
       ((a=B*gugu))    
       echo "$B*$gugu=$a"
    done
    echo " "
done
