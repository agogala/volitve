#!/net/rozle/export/share/sol/bin/python

import socket
import SocketServer

PORT=5002

class Handler(SocketServer.DatagramRequestHandler):
    def handle(self):
	print "Got: %s\n" % self.rfile.readline()

class ObserverServer(SocketServer.UDPServer):
    def server_bind(self):
	SocketServer.UDPServer.server_bind(self)
	self.socket.setsockopt(socket.SOL_SOCK, socket.SO_REUSADDR, 1)

server = SocketServer.UDPServer(('', PORT),Handler)

server.serve_forever()
