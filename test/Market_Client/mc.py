#!/usr/bin/python

from socket import *
import string
import time
import sys

HOST="rozle"
PORT=5001

sock = socket(AF_UNIX, SOCK_STREAM)

sock.connect("/tmp/market")

sock.setsockopt(SOL_SOCKET, SO_KEEPALIVE, 1)

b="BUY KUCA 5 58 miha"

s="BUY KUCA 2 60.1 miha"

while 1:
    sock.send(chr(len(s)) + s)
    response = sock.recv(1024)
    print "Response was: %s" % response

    sock.send(chr(len(b)) + b)
    response = sock.recv(1024)
    print "Response was: %s" % response

    if sys.stdin.readline()[0]=='.':
	break
#    time.sleep(0.1)
    

sock.close()

