#!/bin/bash
#please run script as root
passwd="xen"
total="10G"
free="150M"
min="800M"
delta="400M"
max="8G"
result=output
tests="h2"
#tests="sunflow\ luindex\ lusearch"
repeat=15
xenbus=/dev/xen/xenbus

if [ $# -lt 1 ]; then
   echo "usage: $0 <domain> "
   exit 0
fi

if [ -d $result ]; then
   echo "there are already have *$result* dir, remove it first."
   exit 0
fi

read -sp "input password:" passwd_h
echo ""

dir=`dirname $0`
. $dir/ip.sh
. $dir/open-xs.sh

# force reset memory
echo $passwd_h | sudo -S src/mmserver -r -N $total -f $free &
sleep 10
echo $passwd_h | sudo -S kill `pidof mmserver`

ssh -Tq ${ip[$1]} <<EOF
echo $passwd | sudo -S service mmclientd stop
echo $passwd | sudo -S kill \`pidof mm_test_client\`
echo $passwd | sudo -S XENSTORED_PATH=$xenbus daemon mm_test_client $min $delta $max
renice -10 -p \`pidof mm_test_client\`
EOF

echo $passwd_h | sudo -S src/mmserver -v -N $total -f $free -d $result &

ssh -Tq ${ip[$1]} <<EOF
cd dacapo
./dacapo_test.sh $tests $repeat dacapo
echo $passwd | sudo -S kill \`pidof mm_test_client\`
echo $passwd | sudo -S service mmclientd start
EOF

echo $passwd_h | sudo -S kill `pidof mmserver`
echo $passwd_h | sudo -S chown -R `whoami` $result
scp ${ip[$1]}:dacapo/dacapo.*.log $result

echo "test all finished successful"
echo "all data saved in $result"
