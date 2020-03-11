#!/usr/bin/env python

import sys
import struct

infile = file(sys.argv[1],'rb')
pcount = 0
pcounts = {}
while True:
    cursor = infile.tell()
    header = infile.read(8)
    if len(header) != 8:
        break
    size,pkt_type = struct.unpack('<I4s',header)
    #print size,pkt_type
    if not pkt_type in pcounts:
        pcounts[pkt_type] = 0
    pcounts[pkt_type] += 1
    infile.seek(cursor+size)
    pcount += 1
    if pcount%10000 == 0:
        print pcount,'packets so far...'

print
print pcount,'total packets'
print

keys = pcounts.keys()
keys.sort()
for k in keys:
    print k, pcounts[k]
              
