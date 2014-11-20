#!/bin/bash

dacapo=dacapo-9.12-bach.jar
if [ $# -lt 3 ]; then
    echo "  useage: $0 [all|test id] [times] [output]";
    echo "  example: $0 h2 5 dacapo";
    echo "  example: $0 'h2 sunset' 5 dacapo";
    exit 0
fi
testid="all"
runall=0
file=`date +%F-%s`
times=1
if [ $# -ge 1 ]; then
    testid=$1
fi
if [ $testid == "all" ]; then
    runall=1
    testid=`java -jar $dacapo -l`
fi
if [ $# -ge 2 ]; then
   times=$2
fi
if [ $# -ge 3 ]; then
   file=$3
fi
echo test:$testid runall:$runall repeat:$times
echo "dacapo test set">$file.e.log
touch $file.o.log
for id in $testid
do
    for i in $(seq 1 $times)
    do
    java -jar $dacapo $id 1>>$file.o.log 2>>$file.e.log
    echo "."
    done
done
