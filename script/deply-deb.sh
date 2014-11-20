#!/bin/bash
passwd="xen"
java_pkg="openjdk-7-jre-headless"

read -s -p "input password:" passwd_h

dir=`dirname $0`
. $dir/ip.sh

if [ $# -lt 1 ]; then
   echo "usage: $0 deb-file"
   exit -1
fi

# prepare dacapo
echo "preparing dacapo dir ..."
[ ! -d dacapo ] && mkdir dacapo
cd dacapo
[ ! -e dacapo*.jar ] && wget http://softlayer-dal.dl.sourceforge.net/project/dacapobench/9.12-bach/dacapo-9.12-bach.jar
[ ! -e dacapo_test.sh ] && cp ../../script/dacapo_test.sh .
cd ..

for i in ${!ip[*]}
do
    domid=`virsh domid $i`
    echo $passwd_h | sudo -S xenstore-chmod -r "/local/domain/$domid/memory" b
done

for i in ${ip[*]}
do
    echo "processing $i ..."
    scp -r $1 dacapo $i:
    ssh -Tq $i << EOF
# touch .hushlogin #disable welcome message
echo $passwd | sudo -S dpkg -i $1
echo $passwd | sudo -S apt-get install -yf # fix dependencies
echo $passwd | sudo -S apt-get install $java_pkg -y # fix dependencies
rm $1
echo $passwd | sudo -S service mmclientd restart # restart service
echo $passwd | sudo -S update-rc.d mmclientd defaults 80 80 # add to boot
EOF

done
