# $ProjectHeader: volitve 0.20 Sun, 19 Oct 1997 19:07:54 +0200 andrej $
#
# $Id: Preklic.py 1.2 Sun, 19 Oct 1997 17:07:54 +0000 andrej $
#
# Poskrbi za preklic naroèila.
import sys
import os
import cgi
import time
import Market_Client
import FormatResponse

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
    
    time.sleep(2)
    return FormatResponse.FormatResponse(
	"<H1>Preseljen dokument</H1>\n" + \
	"Dokument je preseljen na drugo lokacijo",
	"Preseljen dokument",
	{'Status' : '302 relocate',
	 'Location' : '/trg/vhod'})

