# $ProjectHeader: volitve 0.27 Fri, 21 Nov 1997 18:06:57 +0100 andrej $
#
# $Id: Preklic.py 1.4 Sat, 08 Nov 1997 07:02:11 +0000 andrej $
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

    uporabnik = os.environ["REMOTE_USER"][:10]

    try:
	for k in form.keys():
	    if form[k].value=='on':
		Market_Client.market_client.send('Cancel %s "%s"' % (k, uporabnik))
		resp = Market_Client.market_client.response()
		# Ignoriramo vse napake...
    except:
	pass

    if Formater_Client.formater_client.open()>=0:
	Formater_Client.formater_client.send('MakeUser "%s"' % uporabnik)
	# Get response, but ignore it:
	Formater_Client.formater_client.response()
    
    return FormatResponse.FormatResponse(
	"<H1>Preseljen dokument</H1>\n" + \
	"Dokument je preseljen na drugo lokacijo",
	"Preseljen dokument",
	{'Status' : '302 relocate',
	 'Location' : '/trg/vhod'})

