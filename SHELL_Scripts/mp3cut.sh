#!/bin/bash

oldIFS=$IFS
IFS=$'\n'

for line in $(cat inputfile);
do
	start=$(echo $line | awk -F '|' '{printf("%s", $1)}')
	end=$(echo $line | awk -F '|' '{printf("%s", $2)}')
	title=$(echo $line | awk -F '|' '{printf("%s", $3)}')

	$(ffmpeg -ss $start -t $end -i $1 -acodec copy "$title.mp3")
	echo $title
done

IFS=$oldIFS
