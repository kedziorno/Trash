#!/usr/bin/python

import sys
import csv
import os
import shutil

if len(sys.argv) < 2:
    print "{} file_name.txt".format(sys.argv[0])
    exit(1)
    
with open(sys.argv[1], 'rt') as file:
    folder_name=file.name.split(".")[0]
    #print folder_name
    
    if os.path.exists(folder_name):
        shutil.rmtree(folder_name)
        
    os.mkdir(folder_name)
    
    line = csv.reader(file, delimiter='|', quotechar='|')
    for l in line:
        #print l[0]
        #print l[1]
        
        cmd1 = "youtube-dl --no-check-certificate --no-playlist -x --audio-format mp3 -o template_file.mp3 '"+l[0]+"'"
        cmd2 = "/usr/bin/ffmpeg -i template_file.mp3 -metadata title='"+l[1]+"' -metadata author='"+l[1]+"' -metadata composer='"+l[1]+"' -codec:a libmp3lame -qscale:a 0 '"+folder_name+"/"+l[1]+".mp3'"
        cmd3 = "rm -f template_file.mp3"
        
        os.system(cmd1)
        os.system(cmd2)
        os.system(cmd3)
        
