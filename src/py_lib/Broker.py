# $ProjectHeader: volitve 0.21 Tue, 21 Oct 1997 21:26:41 +0200 andrej $
#
# $Id: Broker.py 1.5 Sun, 19 Oct 1997 17:07:54 +0000 andrej $
#
# Prevede podatke iz obrazca v zahtevek za trg.
#import Apache
import sys
import os
import cgi
import Market_Client
import CheckForm
import string
import FormatResponse

rel_msg = \
	"<H1>Preseljen dokument</H1>\n" + \
	"Dokument je preseljen na drugo lokacijo"
rel_title = "Preseljen dokument",
rel_headers = {'Status' : '302 relocate',
	       'Location' : '/narocila/dodaj.html'}


def HandleForm(form):
    if Market_Client.market_client.open()<0:
	return FormatResponse.FormatResponse(
	    "<H1>Zaprt trg</H1>\n" + \
	    "Trg je trenutno zaprt\n",
	    "Zaprt trg")
    
    # Preverimo veljavnost podatkov:

    # Polja: Ponudnik je zaèasen
    fields = ["Papir", "Vrsta", "Kolicina", "Cena"]
    reqfields = fields

    if not CheckForm.CheckForm(form, fields, reqfields):
	return FormatResponse.FormatResponse(
	    "<H1>Napaka</H1>\n"  + \
	    "Prosim izpolnite vsa polja...",
	    "Napaka")

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
	return FormatResponse.FormatResponse(
	    "<H1>Napaka</H1>" + \
	    sys.exc_type + sys.exc_value + sys.exc_traceback,
	    "Napaka")

    #    msg =  "Po¹iljam: %s<P>" % zahtevek 

    headers = {}
    title = ""
    msg = ""

    try:
	Market_Client.market_client.send(zahtevek)
	response = Market_Client.market_client.response()
	if response=="000":
	    msg = rel_msg
	    title = rel_title
	    headers = rel_headers
	else:
	    # Obrazlo¾itve...
	    msg = '<H1>Napaka</H1>\n %s' % response
    except:
	#	pass
	msg = "<H1>Zaprt trg</H1>\n" + \
	      "Trg je trenutno zaprt\n"
	
    #    Count.increase()
    #    msg = msg + "©tevilka: %d<P>" % Count.value()
    return FormatResponse.FormatResponse(
	msg, title, headers
	)
    
