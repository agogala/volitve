# $ProjectHeader: volitve 0.10 Thu, 11 Sep 1997 18:28:32 +0200 andrej $
#
# $Id: Broker.py 1.3 Wed, 10 Sep 1997 16:15:50 +0000 andrej $
#
# Prevede podatke iz obrazca v zahtevek za trg.
#import Apache
import sys
import os
import cgi
import Count
import Market_Client
import CheckForm
import string

def HandleForm(form):
    if Market_Client.market_client.open()<0:
	return "<H1>Zaprt trg</H1>\n" + \
	       "Trg je trenutno zaprt\n"
    
    # Preverimo veljavnost podatkov:

    # Polja: Ponudnik je zaèasen
    fields = ["Papir", "Vrsta", "Kolicina", "Cena"]
    reqfields = fields

    if not CheckForm.CheckForm(form, fields, reqfields):
	return "<H1>Napaka</H1>\n"  + \
	       "Prosim izpolnite vsa polja..."

    try:
	if form["Vrsta"].value == "Nakup":
	    vrsta = "BUY"
	elif form["Vrsta"].value == "Prodaja":
	    vrsta = "SELL"
	else:
	    raise "Napaèna vrsta naroèila: %s" % form["Vrsta"].value

	# Papir kar privzamemo, zaenkrat [...]
	papir = form["Papir"].value

	# Kolièina:
	kol = string.atoi(form["Kolicina"].value)
	
	cena = string.atof(form["Cena"].value)
	
	#	ponudnik = form["Ponudnik"].value
	ponudnik = os.environ["REMOTE_USER"]

	zahtevek = "%(vrsta)s %(papir)s %(kol)s %(cena)s %(ponudnik)s" % locals()
    except: 
	# Tule bi moral poroèati kaj je narobe:
	return "<H1>Napaka</H1>" + \
	       sys.exc_type + sys.exc_value + sys.exc_traceback

    msg =  "Po¹iljam: %s<P>" % zahtevek 

    try:
	Market_Client.market_client.send(zahtevek)
	response = Market_Client.market_client.response()
	msg = msg + "Odgovor je: %s<P>" % response
    except:
	msg = msg + "Trg se je zaprl...<P>"

    Count.increase()
    msg = msg + "©tevilka: %d<P>" % Count.value()
    return msg
    
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
