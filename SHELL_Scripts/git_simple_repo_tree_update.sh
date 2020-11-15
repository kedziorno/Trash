#!/bin/sh

COMMENT=""
if [ -n $1 ]
then
	COMMENT="$1"
fi

#LOG_FILE="`basename -s .sh $0`.txt"
LOG_FILE=/dev/null
DATE_START="`date \"+%Y-%m-%d %H:%M:%S\"`"
echo "* Script start at $DATE_START" | tee -a $LOG_FILE 
if [ $COMMENT ]
then
echo "(Comment: $COMMENT)" | tee -a $LOG_FILE
fi
for i in `find . -maxdepth 2 -type d -name ".git" -exec dirname {} \;`; do echo "Enter $i..." | tee -a $LOG_FILE; (cd $i && git fetch && git merge) | tee -a $LOG_FILE; done
DATE_END="`date \"+%Y-%m-%d %H:%M:%S\"`"
echo "* Script ended at $DATE_END\n\n\n" | tee -a $LOG_FILE
