#!/bin/bash

dacapo=dacapo-9.12-bach.jar
file=`date +%F-%s.log`
if [ $# -lt 2 ]; then
    echo "  useage: $0 [all|test id] [times]";
    echo "  example: $0 h2 5";
    echo "  example: $0 'h2 sunset' 5";
    exit 0
fi
testid="all"
runall=0
times=1
if [ $# -ge 1 ]; then
    testid=$1
fi
if [ $testid == "all" ]; then
    runall=1
    testid=`java -jar $dacapo -l`
fi
if [ $# -eq 2 ]; then
   times=$2
fi
echo $testid,$runall,$times
echo "dacapo test set">$file
for id in $testid
do
    for i in $(seq 1 $times)
    do
    java -jar $dacapo $id 2>>$file
    echo "."
    done
done
