gugu=1
while ((gugu < 10))
do
    ((a=2*gugu))
    echo "2*$gugu=$a"
    ((gugu++))
done
