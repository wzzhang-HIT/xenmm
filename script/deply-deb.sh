#!/bin/bash

dir=`dirname $0`
. $dir/ip.sh
for i in ${ip[*]}
do
    echo "processing $i"
    scp $1 $i:
    ssh -Tq $i << EOF
# touch .hushlogin #disable welcome message
echo "xen" | sudo -S dpkg -i $1
rm $1
echo "xen" | sudo -S service mmclientd restart # restart service
echo "xen" | sudo -S update-rc.d mmclientd defaults 80 80 # add to boot
EOF

done
