# $ProjectHeader: volitve 0.28 Sat, 27 Dec 1997 16:06:49 +0100 andrej $
#
# $Id: Pogodbe.py 1.1 Sat, 08 Nov 1997 07:02:11 +0000 andrej $
#
# Prikaz sklenjenih pogodb za posameznega kandidata.

import Uporabniki
import Formater_Client
import os
import cgi_config

rel_msg = \
"Status: 302 relocate\n" \
"Location: /uporabniki/%s/%s.html\n" \
"Content-Type: text/plain\n" \
"\n" \
"Document relocated\n" 

def HandleForm(form):
    user = os.environ['REMOTE_USER'][:10]
    # Path...
    paper = os.environ['PATH_INFO'][1:9]
    oid = Uporabniki.UserID(user)
    indexfile = cgi_config.htmldir + '/uporabniki/%s/index.html' % oid
    pogodbfile = cgi_config.htmldir + '/uporabniki/%s/%s.html' % (oid,paper)
    doit = 1
    try:
	istat = os.stat(indexfile)
	pstat = os.stat(pogodbfile)
	doit = istat[8]>pstat[8]
    except:
	pass

    if doit and Formater_Client.formater_client.open()>=0:
	Formater_Client.formater_client.send('Contracts "%s" "%s"' % (user,paper))
	## Get response, but ignore it:
	Formater_Client.formater_client.response()	
    print rel_msg % (oid, paper)
