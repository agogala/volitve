#!/net/rozle/export/share/sol/bin/python

import socket
import SocketServer
import MakePregled
import admin_cfg

class Handler(SocketServer.DatagramRequestHandler):
    def handle(self):
	print "Got: %s\n" % self.rfile.readline()
	MakePregled.run(admin_cfg.tempdir, 
			admin_cfg.htmldir, admin_cfg.templates)

class ObserverServer(SocketServer.UDPServer):
    def server_bind(self):
	SocketServer.UDPServer.server_bind(self)
	self.socket.setsockopt(socket.SOL_SOCK, socket.SO_REUSADDR, 1)

def run(port):
    MakePregled.run(admin_cfg.tempdir, admin_cfg.htmldir, admin_cfg.templates) 
    server = SocketServer.UDPServer(('', port), Handler)
    server.serve_forever()
