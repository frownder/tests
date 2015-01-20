IFS=:

while read username passwd uid gid
do
    echo $username $uid
done < /etc/passwd
