#!/net/rozle/export/share/sol/bin/python

from socket import *
import string

HOST="rozle"
PORT=5002

sock = socket(AF_INET, SOCK_STREAM)

sock.setblocking(0)

sock.connect(HOST, PORT)

while 1:
    select([sock],[],[]) 
    response = sock.recv(1024)
    print response[1:] + "\n"

sock.close()

print "Response was: %s" % response
