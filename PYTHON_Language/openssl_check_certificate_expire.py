#!/usr/bin/env python3

import os
import sys
import subprocess

OPENSSL = os.getenv('OPENSSL','openssl')
       
def main1():
    if len(sys.argv) < 3:
        sys.exit('Usage: %s [host] [days]' % sys.argv[0])
    
    host = sys.argv[1]
    daysInSeconds = int(sys.argv[2])*24*3600
    
    command_line = 'echo | %s s_client  -connect %s:443 -verify_hostname %s 2>/dev/null | sed -n "/-----BEGIN CERTIFICATE-----/,/-----END CERTIFICATE-----/p" | %s x509 -checkend %d' % (OPENSSL, host, host, OPENSSL, daysInSeconds)
    #print(command_line)
    
    process = os.popen(command_line)
    
    print(process.read())

if __name__ == '__main__':
    main1()
