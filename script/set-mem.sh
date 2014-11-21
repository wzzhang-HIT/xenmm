#!/bin/bash

dir=`dirname $0`
. $dir/ip.sh

mem="1G"
max="2G"
domain=""

if [ $# -eq 2 ]; then
   domain="${!ip[*]}"
   mem=$1
   max=$2
elif [ $# -ge 3 ]; then
   domain=$1
   mem=$2
   max=$3
else
   echo "usage: $0 <init-mem> <max-mem>"
   echo "usage: $0 <domain> <init-mem> <max-mem>"
   echo "example: $0 1024M 2048M"
   echo "example: $0 ubuntu_0 1024M 2048M"
   exit -1
fi

for i in ${domain}
do
    virsh setmaxmem $i $max --live --config
    virsh setmem $i $mem --live --config
    virsh destroy $i
    virsh start $i
    virsh setmaxmem $i $max --live --config
    virsh setmem $i $mem --live --config
done

echo NOTE: sometimes auto configure failed, use *xl top* to check valid.
