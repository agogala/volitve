#import Apache
import sys
import cgi
import Count
import Market_Client
import CheckForm
import string

def HandleForm(form):
    print "Content-type: text/html; charset=iso-8859-2"
    print 
    
    if Market_Client.market_client.open()<0:
	print "<H1>Zaprt trg</H1>"
	print "Trg je trenutno zaprt\n"
	return
    
    # Preverimo veljavnost podatkov:

    # Polja: Ponudnik je zaèasen
    fields = ["Papir", "Vrsta", "Kolicina", "Cena", "Ponudnik"]
    reqfields = fields

    if not CheckForm.CheckForm(form, fields, reqfields):
	print "<H1>Napaka</H1>"
	print "Prosim izpolnite vsa polja..."
	return

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
	
	ponudnik = form["Ponudnik"].value

	zahtevek = "%(vrsta)s %(papir)s %(kol)s %(cena)s %(ponudnik)s" % locals()
    except:
	print "<H1>Napaka</H1>"
	print sys.exc_type, sys.exc_value, sys.exc_traceback
	return

    print "Po¹iljam: %s<P>" % zahtevek 

    Market_Client.market_client.send(zahtevek)
    response = Market_Client.market_client.response()
    print "Odgovor je: %s<P>" % response

    Count.increase()
    print "©tevilka: %d<P>" % Count.value()

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
