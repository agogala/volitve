#!/usr/bin/python

from socket import *
import string
import time
import sys

HOST="rozle"
PORT=5001

sock = socket(AF_UNIX, SOCK_STREAM)

sock.connect("/tmp/market")

s="BUY KUCA 10 60.1 Crt"

while 1:
    sock.send(chr(len(s)) + s)
    response = sock.recv(1024)
    print "Response was: %s" % response
    if sys.stdin.readline()[0]=='.':
	break
#    time.sleep(0.1)
    

sock.close()

