#!/bin/bash
#please run script as root
passwd="xen"
total="5G"
free="150M"

. ip.sh

# force reset memory
src/mm_server -r -N $total -f $free &
sleep 5
kill `pidof mm_server`

ssh ${ip[$1]} <<EOF
echo $passwd | sudo -S service mmclientd stop
echo $passwd | mm_test_client $2 $3 &
renice -10 -p `pidof mm_test_client`
EOF

src/mmserver -N $total -f $free &

ssh ${ip[$1]} <<EOF
./dacapo_test 'sunflow luindex lusearch' 15
echo $passwd | kill `pidof mm_test_client`
echo $passwd | sudo -S service mmclientd start
EOF

kill `pidof mmserver`
