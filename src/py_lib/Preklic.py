# $ProjectHeader: volitve 0.19 Thu, 09 Oct 1997 15:19:34 +0200 andrej $
#
# $Id: Preklic.py 1.1 Thu, 09 Oct 1997 13:19:34 +0000 andrej $
#
# Poskrbi za preklic naroèila.
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
    
