#!/bin/sh

OUTPUT_FILE="out.mp3"

for LIST_FOLDERS in `find . -type d -maxdepth 1 | cut -b3- | sort`;
do
    #echo ${LIST_FOLDERS};
    ( 
        cd ${LIST_FOLDERS};
        ls -1 -r --sort time *no*.mp3 | xargs printf "file '%s'\n" > list_mp3.txt;
        /usr/bin/ffmpeg -f concat -safe 0 -i list_mp3.txt -c copy "${LIST_FOLDERS}_output.mp3"
        rm -f list_mp3.txt
    )
done
