cat /etc/passwd | nawk -F: '\
NF != 7{ \
printf("line %d, does not have 7 fields: %s\n", NR,$0)} \
$1 !~ /[A-Za-z0-9]/{printf("line %d, nonalahnumeric user id: %s\n", NR,$0)} \
$2 == "x"{printf("line %d, no password: %s\n",NR,$0)}'
