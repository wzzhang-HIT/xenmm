#!/bin/bash
passwd="xen"

dir=`dirname $0`
. $dir/ip.sh

if [ $# -le 1 ]; then
   echo "usage: $0 deb-file"
   exit -1
fi

# prepare dacapo
if [ ! -d dacapo ]; then
   mkdir dacapo
   cd dacapo
   wget http://softlayer-dal.dl.sourceforge.net/project/dacapobench/9.12-bach/dacapo-9.12-bach.jar
   [ ! -e dacapo_test.sh ] && cp ../../script/dacapo_test.sh .
   cd ..
fi

for i in ${ip[*]}
do
    echo "processing $i"
    scp -r $1 dacapo $i:
    ssh -Tq $i << EOF
# touch .hushlogin #disable welcome message
echo $passwd | sudo -S dpkg -i $1
rm $1
echo $passwd | sudo -S service mmclientd restart # restart service
echo $passwd | sudo -S update-rc.d mmclientd defaults 80 80 # add to boot
EOF

done
