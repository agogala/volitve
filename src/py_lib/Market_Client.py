# $ProjectHeader: volitve 0.9 Wed, 10 Sep 1997 18:15:50 +0200 andrej $
#
# $Id: Market_Client.py 1.3 Fri, 05 Sep 1997 14:43:33 +0000 andrej $
#
# Objekt, ki definira povezavo s trgom
import socket
import cgi_config

class MarketClient:
    def __init__(self):
        self.sock = None
        pass

    def open(self):
        if self.sock==None:         
            self.sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
            try:
                self.sock.connect(cgi_config.MARKET_PATH)
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
	# Aha!, market se je odklopil
	if resp == "":
	    self.sock.close()
	    self.sock = None
	    resp = " Trg se je zaprl"
	return resp[1:]

market_client = MarketClient()
