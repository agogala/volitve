# $ProjectHeader: volitve D.2 Wed, 03 Sep 1997 07:39:11 +0200 andrej $
#
# $Id: Market_Client.py 1.1 Fri, 29 Aug 1997 11:44:29 +0000 andrej $
#
# Objekt, ki definira povezavo s trgom
import socket
import config

class MarketClient:
    def __init__(self):
        self.sock = None
        pass

    def open(self):
        if self.sock==None:         
            self.sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
            try:
                self.sock.connect(config.MARKET_PATH)
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
