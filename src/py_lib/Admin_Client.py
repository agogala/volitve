# $ProjectHeader: volitve 0.27 Fri, 21 Nov 1997 18:06:57 +0100 andrej $
#
# $Id: Admin_Client.py 1.1 Thu, 11 Sep 1997 16:28:32 +0000 andrej $
#
# Objekt, ki definira povezavo s trgom
import socket
import cgi_config

class AdminClient:
    def __init__(self):
        self.sock = None
        pass

    def open(self):
        if self.sock==None:         
            self.sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
            try:
                self.sock.connect(cgi_config.ADMIN_PATH)
            except:
		self.sock = None
                return -1
        return 0

    def send(self, msg):
        if len(msg)>255:
            raise "Message len>255"
	try:
	    self.sock.send(chr(len(msg))+msg)
	except:
	    self.sock.close()
	    self.sock = None
	    raise sys.exc_type, sys.exc_value, sys.exc_traceback

    def response(self):
	resp = self.sock.recv(1024)
	# Aha!, administrator se je odklopil
	if resp == "":
	    self.sock.close()
	    self.sock = None
	    raise "Administrator ne teèe"
	return resp[1:]

admin_client = AdminClient()
