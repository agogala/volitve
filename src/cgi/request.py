#import Apache
import sys
import Count

#m = Market_Client.MarketClient()

print "Content-type: text/plain; charset=iso-8859-2"
print

import Market_Client


#if m.open():
if Market_Client.market_client.open()<0:
    print "Trg je trenutno zaprt\n"
    sys.exit(0)

# Preberi zahtevek:

s="BUY KUCA 10 60.1 Crt"

print "Po�iljam: %s\n" % s

#m.send(s)
Market_Client.market_client.send(s)
#response = m.response()
response = Market_Client.market_client.response()
print "Odgovor je: %s\n" % response

Count.increase()
print "�tevilka: %d\n" % Count.value()

#sys.exit(0)

# NOTREACHED:
## r = Apache.request
## print "Request's doc: ", r.__doc__
## print "Request's text: ", r.the_request
## print "Request's members: ", r.__members__
## print "Remote host: ", r.get_remote_host()
## try:
##     print r.get_remote_logname()
## except Apache.error, m:
##     print "Cannot get remote logname: ", m

## c = r.connection
## print "Connection's doc: ", c.__doc__
## print "Connection's remote IP: ", c.remote_ip
## print "Connection's members: ", c.__members__

## print "Headers in: ", r.headers_in.items()
## print "Headers out: ", r.headers_out.items()
## print "Err Headers out: ", r.err_headers_out.items()
## print "Subprocess env: ", r.subprocess_env.items()
## print "Notes: ", r.notes.items()

## s = c.server
## print "Server's doc: ", s.__doc__
## print "Server's admin: ", s.server_admin
## print "Server's members: ", s.__members__
