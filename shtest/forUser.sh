USERS=$(who | awk '{print $1}' | sort | uniq)

for user in $USERS
do
    echo "mail" $user "<Hello this is yong"

#    mail $user <<- ENDFILE  #- eliminates TAB
#    hi
#    bye
#    ENDFILE    # safe on 1 column
done
