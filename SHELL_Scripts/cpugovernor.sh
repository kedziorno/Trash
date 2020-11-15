#!/bin/sh

CPUS=`cat /proc/cpuinfo | grep processor | awk -F ' ' '{printf("%s ",$3);}'`
CPUPATH="/sys/bus/cpu/devices"

for i in ${CPUS}; do
	CPUPATHFREQGOVERNOR="${CPUPATH}/cpu${i}/cpufreq/scaling_governor"
        echo -n "CPU${i}: `cat ${CPUPATHFREQGOVERNOR}` "
done
