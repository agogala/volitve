# $ProjectHeader: volitve 0.23 Tue, 28 Oct 1997 21:15:29 +0100 andrej $
#
# $Id: Formater_Client.py 1.1 Tue, 28 Oct 1997 20:15:29 +0000 andrej $
#
# Objekt, ki definira povezavo z oblikovalcem
import socket
import cgi_config

class FormaterClient:
    def __init__(self):
        self.sock = None
        pass

    def open(self):
        if self.sock==None:         
            self.sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
            try:
                self.sock.connect(cgi_config.FORMATER_PATH)
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
	    raise "Formater ne teèe"
	return resp[1:]

formater_client = FormaterClient()
