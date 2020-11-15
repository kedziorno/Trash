#!/bin/sh

CPUS=`cat /proc/cpuinfo | grep processor | awk -F ' ' '{printf("%s ",$3);}'`

#F=$1
F=2400
for i in ${CPUS}; do
	echo "CPU $i: set max ${F} KHz";
	FREQ=`cpufreq-set -c ${i} -u 2.4GHz -g performance`
done
