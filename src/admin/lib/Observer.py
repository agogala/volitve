# $ProjectHeader: volitve 0.18 Sun, 05 Oct 1997 22:53:12 +0200 andrej $
#
# $Id: Observer.py 1.2 Fri, 05 Sep 1997 14:43:33 +0000 andrej $
# Observer deamon. Ne dela, ker je SocketServer zaniè.
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
