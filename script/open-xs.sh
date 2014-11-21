#!/bin/bash
echo "setting up xenstore permissions..."
dir=`dirname $0`
. $dir/ip.sh

for i in ${!ip[*]}
do
    domid=`virsh domid $i`
    echo $passwd_h | sudo -S xenstore-chmod -r "/local/domain/$domid/memory" b
done
echo "waiting vms writes xenstore..."
sleep 20 # wait all vm write xenstore success
