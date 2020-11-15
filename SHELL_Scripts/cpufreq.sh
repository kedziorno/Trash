#!/bin/sh

CPUS=`cat /proc/cpuinfo | grep processor | awk -F ' ' '{printf("%s ",$3);}'`

for i in ${CPUS}; do
	FREQ=`cpufreq-info -c ${i} -f`
	echo "$i: $FREQ";
done
