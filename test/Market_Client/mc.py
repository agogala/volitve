#!/usr/bin/python

from socket import *
import string
import time
import sys

def send(sock, r):
    sock.send(chr(len(r)) + r)
    response = sock.recv(1024)
    print "Response was: %s" % response


HOST="rozle"
PORT=5001

sock = socket(AF_UNIX, SOCK_STREAM)

sock.connect("/tmp/market-d")

sock.setsockopt(SOL_SOCKET, SO_KEEPALIVE, 1)

#send(sock, "BUY KUCA 4 50 Tomo")

#send(sock, "SELL KUCA 72 1 Mitja")

#send(sock, "SELL PERS 72 25 Mitja")

send(sock, 'BUY PERS 5 25 "Matej Nova"')

#send(sock, "CANCEL 627458 Tomo")


## while 1:
##     sock.send(chr(len(s)) + s)
##     response = sock.recv(1024)
##     print "Response was: %s" % response

##     sock.send(chr(len(b)) + b)
##     response = sock.recv(1024)
##     print "Response was: %s" % response

##     if sys.stdin.readline()[0]=='.':
## 	break
## #    time.sleep(0.1)
    

sock.close()

