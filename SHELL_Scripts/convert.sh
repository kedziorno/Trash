#!/bin/sh

EXTENSION_NAME="txt"
AUDIO_EXTENSION_NAME="mp3"
INDEX_FILENAME_START=1
RM_INTERACTIVE="" #-i

for FILES_TXT in `ls *.${EXTENSION_NAME}`;
do
    FILE_NAME=${FILES_TXT};
    NAME=`basename ${FILE_NAME} .${EXTENSION_NAME}`;
    INDEX=${INDEX_FILENAME_START};
    
    FOLDER_NAME=${NAME};
    
    NR_LINKS=`cat ${FILE_NAME} | wc -l`
    
    if [ ${NR_LINKS} -eq 0 ];
    then
        continue;
    fi
    
    mkdir ${FOLDER_NAME};
    
    for LINK_ORIGINAL in `awk -F "\n" '{print $1}' ${FILE_NAME} | grep -v '^#'`;
    do
        LINK=`echo ${LINK_ORIGINAL} | awk -F "&list=" '{print $1}'`;
        MP3_NAME="${NAME}_no${INDEX}.${AUDIO_EXTENSION_NAME}";
        
        PATH_OUT="${FOLDER_NAME}/${MP3_NAME}";
        
        #echo ${PATH_OUT};
        #echo ${LINK};
        
        #youtube-dl -f bestaudio  "${LINK}" --exec "/usr/bin/ffmpeg -i {}  -codec:a libmp3lame -qscale:a 0 ${PATH_OUT}"
        
        youtube-dl --no-playlist -x --audio-format mp3 -o template_file.mp3 "${LINK}" 
        /usr/bin/ffmpeg -i template_file.mp3 -codec:a libmp3lame -qscale:a 0 ${PATH_OUT}
        rm -f template_file.mp3
        
        INDEX=`expr ${INDEX} + 1`;
    done
done
