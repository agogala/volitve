# $ProjectHeader: volitve 0.24 Mon, 03 Nov 1997 14:25:50 +0100 andrej $
#
# $Id: Preklic.py 1.3 Tue, 28 Oct 1997 20:15:29 +0000 andrej $
#
# Poskrbi za preklic naroèila.
import sys
import os
import cgi
import time
import Market_Client
import FormatResponse
import Formater_Client

def HandleForm(form):
    if Market_Client.market_client.open()<0:
	return FormatResponse.FormatResponse("<H1>Zaprt trg</H1>\n" + \
	       "Trg je trenutno zaprt\n", "Zaprt trg")

    uporabnik = os.environ["REMOTE_USER"]

    try:
	for k in form.keys():
	    if form[k].value=='on':
		Market_Client.market_client.send("Cancel %s %s" % (k, uporabnik))
		resp = Market_Client.market_client.response()
		# Ignoriramo vse napake...
    except:
	pass

    if Formater_Client.formater_client.open()>=0:
	Formater_Client.formater_client.send("MakeUser %s" % uporabnik)
	# Get response, but ignore it:
	Formater_Client.formater_client.response()
    
    return FormatResponse.FormatResponse(
	"<H1>Preseljen dokument</H1>\n" + \
	"Dokument je preseljen na drugo lokacijo",
	"Preseljen dokument",
	{'Status' : '302 relocate',
	 'Location' : '/trg/vhod'})

