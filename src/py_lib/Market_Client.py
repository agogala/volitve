# $ProjectHeader: volitve 0.5 Thu, 04 Sep 1997 16:58:57 +0200 andrej $
#
# $Id: Market_Client.py 1.2 Thu, 04 Sep 1997 14:58:57 +0000 andrej $
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
                return -1
        return 0

    def send(self, msg):
        if len(msg)>255:
            raise "Message len>255"
        self.sock.send(chr(len(msg))+msg)
        pass

    def response(self):
        return self.sock.recv(1024)[1:]

market_client = MarketClient()
