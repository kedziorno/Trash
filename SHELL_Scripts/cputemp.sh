TEMPERATURECPU=`cat /sys/devices/virtual/thermal/thermal_zone0/temp`
T_CPU=`expr $TEMPERATURECPU / 1000`
echo "$T_CPU st C"
