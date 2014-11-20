#!/bin/bash

dir=`dirname $0`
. $dir/ip.sh

if [ $# -lt 2 ]; then
    echo "usage: $0 <init-mem> <max-mem>"
    echo "example: $0 1024M 2048M"
    exit -1
fi

for i in ${!ip[*]}
do
    virsh setmaxmem $i $2 --live --config
    #virsh setmem $i $1 --live --config
    # it seems no need reboot
    #state=`virsh domstate $i`
    #if [ $state = "running" ]; then
    #    virsh reboot $i
    #elif [ $state = "shut off" ]; then
    #    virsh start $i
    #fi
    #sleep 5
done

echo NOTE: sometimes auto configure failed, use *xl top* to check valid.
