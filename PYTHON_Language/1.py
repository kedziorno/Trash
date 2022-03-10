#!/usr/bin/python

#convert rom image 10bit x 26bit to 2x DEEDS ROMs 1Kx16bit

import math

a = "12f0401"

#print ("fff ", a)

#res = "{0:08b}".format(int(a,26))
res = bin(int(a,16))

#print ("c ", str(res))
#print ("b ", str(res)[2:])
#print ("c ", str(res)[20:25])

res = str(res)[2:]
print ("%s.%s.%s.%s.%s\n" % (str(res)[0:4],str(res)[4:8],str(res)[8:12],str(res)[12:16],str(res)[16:20]))

file1 = open('aaa','r')
Lines = file1.readlines()

file2 = open('ROM1.drs','w')
file3 = open('ROM2.drs','w')

for line in Lines:
	a = line.strip().split(" ");
	for i in a:
		res = bin(int(i,16))
		res = str(res)[2:]
		print ("a ", i)
		print ("b ", res)
		h1 = "#- %s\n" % (i)
		file2.write(h1)
		a = "%c%c%c%c.%c%c%c%c.%c%c%c%c.%c%c%c%c\n" % (str(res)[-16],str(res)[-15],str(res)[-14],str(res)[-13],str(res)[-12],str(res)[-11],str(res)[-10],str(res)[-9],str(res)[-8],str(res)[-7],str(res)[-6],str(res)[-5],str(res)[-4],str(res)[-3],str(res)[-2],str(res)[-1])
		file2.write(a)
		file3.write(h1)
		if len(res) == 23:
			b = "0000.0000.0%c%c%c.%c%c%c%c\n" % (str(res)[-23],str(res)[-22],str(res)[-21],str(res)[-20],str(res)[-19],str(res)[-18],str(res)[-17])
			file3.write(b)
		if len(res) == 24:
			b = "0000.0000.%c%c%c%c.%c%c%c%c\n" % (str(res)[-24],str(res)[-23],str(res)[-22],str(res)[-21],str(res)[-20],str(res)[-19],str(res)[-18],str(res)[-17])
			file3.write(b)
		if len(res) == 25:
			b = "0000.000%c.%c%c%c%c.%c%c%c%c\n" % (str(res)[-25],str(res)[-24],str(res)[-23],str(res)[-22],str(res)[-21],str(res)[-20],str(res)[-19],str(res)[-18],str(res)[-17])
			file3.write(b)
		if len(res) == 26:
			b = "0000.00%c%c.%c%c%c%c.%c%c%c%c\n" % (str(res)[-26],str(res)[-25],str(res)[-24],str(res)[-23],str(res)[-22],str(res)[-21],str(res)[-20],str(res)[-19],str(res)[-18],str(res)[-17])
			file3.write(b)
		
file2.close()
file3.close()


