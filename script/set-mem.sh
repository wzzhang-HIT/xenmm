#!/bin/bash
#please run script as root

dir=`dirname $0`
. $dir/ip.sh

if [ $# -lt 2 ]; then
    echo "usage: $0 <init-mem(MB)> <max-mem(MB)>"
    echo "example: $0 1024 2048"
    exit -1
fi

for i in ${!ip[*]}
do
    echo "processing $i ..."
    xm mem-max $i $2
    xm mem-set $i $1
    uptime=`xm uptime $i | grep -oP '\d+:\d+:\d+'`
    if [ $uptime = "0:00:00" ]; then
        xm start $i
    else
        xm reboot $i
    fi
    sleep 60
done

echo "sometimes not reboot successful."
echo "use #xm list to check and reboot manual."
