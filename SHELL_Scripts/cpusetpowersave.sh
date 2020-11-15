#!/bin/sh

CPUS=`cat /proc/cpuinfo | grep processor | awk -F ' ' '{printf("%s ",$3);}'`

F="powersave"
for i in ${CPUS}; do
	echo "CPU $i: set ${F}";
	cpufreq-set -c ${i} -g ${F}
done
