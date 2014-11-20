#!/bin/bash
#please run script as root
passwd="xen"
total="10G"
free="150M"
#tests="sunflow luindex lusearch"
tests="sunflow\ luindex\ lusearch"
#repeat=15
repeat=15
xenbus=/dev/xen/xenbus

if [ $# -lt 3 ]; then
   echo "usage: $0 <domain> <base> <delta>"
   exit 0
fi

read -sp "input password:" passwd_h

dir=`dirname $0`
. $dir/ip.sh

for i in ${!ip[*]}
do
    domid=`virsh domid $i`
    echo $passwd_h | sudo -S xenstore-chmod -r "/local/domain/$domid/memory" b
done

# force reset memory
echo $passwd_h | sudo -S src/mmserver -r -N $total -f $free &
sleep 5
echo $passwd_h | sudo -S kill `pidof mmserver`

ssh -Tq ${ip[$1]} <<EOF
echo $passwd | sudo -S service mmclientd stop
echo $passwd | sudo -S env XENSTORED_PATH=$xenbus mm_test_client $2 $3 &
renice -10 -p `pidof mm_test_client`
EOF

echo $passwd_h | sudo -S src/mmserver -v -N $total -f $free -d output &

ssh -Tq ${ip[$1]} <<EOF
cd dacapo
echo ./dacapo_test.sh \'$tests\' $repeat
./dacapo_test.sh \'$tests\' $repeat
echo $passwd | sudo -S kill `pidof mm_test_client`
echo $passwd | sudo -S service mmclientd start
EOF

echo $passwd_h | sudo -S kill `pidof mmserver`
