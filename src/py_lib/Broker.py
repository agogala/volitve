# $ProjectHeader: volitve 0.25 Tue, 04 Nov 1997 19:56:32 +0100 andrej $
#
# $Id: Broker.py 1.8 Tue, 04 Nov 1997 18:56:32 +0000 andrej $
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
import cgi_config
from ErrorText import MarketErrorText

rel_msg = \
	"<H1>Preseljen dokument</H1>\n" + \
	"Dokument je preseljen na drugo lokacijo"
rel_title = "Preseljen dokument",
rel_headers = {'Status' : '302 relocate',
	       'Location' : '/narocila/dodaj.html'}

back_msg = '<hr><a href="/narocila/dodaj.html">Nazaj</a>'

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
	    "Prosim izpolnite vsa polja." + back_msg,
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
	try:
	    kol = string.atoi(form["Kolicina"].value)
	except:
	    return FormatResponse.FormatResponse(
		"<H1>Napaka</H1> Kolièina nima prave vrednosti." + back_msg)

	try:
	    cena = string.atof(ReformatFloat(form["Cena"].value))
	except:
	    return FormatResponse.FormatResponse(
		"<H1>Napaka</H1> Cena nima prave vrednosti." + back_msg,
		"Napaka")
		
	
	#	ponudnik = form["Ponudnik"].value
	ponudnik = os.environ["REMOTE_USER"][:10]

	zahtevek = '%(vrsta)s %(papir)s %(kol)s %(cena)s "%(ponudnik)s"' % locals()
    except: 
	# Tule bi moral poroèati kaj je narobe:
	return FormatResponse.FormatResponse(
	    "<H1>Napaka</H1>" + \
	    sys.exc_type,
	    "Napaka")

    #    msg =  "Po¹iljam: %s<P>" % zahtevek 

    headers = {}
    title = ""
    msg = ""

    try:
	Market_Client.market_client.send(zahtevek)
	response = Market_Client.market_client.response()
	if response=="000":
	    import Formater_Client
	    try:
		if Formater_Client.formater_client.open()>=0:
		    Formater_Client.formater_client.send("MakePregled")
		    # Get response, but ignore it:
		    Formater_Client.formater_client.response()
	    except:
		pass
## 	    try:
## 		f = open(cgi_config.htmldir + '/narocila/pregled.html', "r")
## 		msg = f.read()
## 		f.close()
## 		print "Content-Type: text/html"
## 		print "Window-target: pregled"
## 		print "Content-Length: %d" % len(msg)
## 		print
## 		print msg
## 		return
## 	    except:
	    msg = rel_msg
	    title = rel_title
	    headers = rel_headers
	else:
	    # Obrazlo¾itve...
	    try:
		msg = '<H1>Napaka</H1>\n' + MarketErrorText[response] + back_msg
	    except:
		msg = '<H1>Napaka</H1>\n Pri¹lo je do napake: %s' % response + back_msg
    except:
	#	pass
	msg = "<H1>Zaprt trg</H1>\n" + \
	      "Trg je trenutno zaprt. Poskusite kasneje." + back_msg
	
    #    Count.increase()
    #    msg = msg + "©tevilka: %d<P>" % Count.value()
    return FormatResponse.FormatResponse(
	msg, title, headers
	)
    
def ReformatFloat(s):
    r = ''
    for i in s:
	if i=='.':
	    r = r + ','
	elif i==',':
	    r = r + '.'
	else:
	    r = r + i
    return r
