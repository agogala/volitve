# $ProjectHeader: volitve 0.25 Tue, 04 Nov 1997 19:56:32 +0100 andrej $
#
# $Id: Redirect.py 1.4 Mon, 03 Nov 1997 13:25:50 +0000 andrej $
#
# Preusmeri uporabnika v njegov direktorij.

import Uporabniki
import Formater_Client
import os

rel_msg = \
"Status: 302 relocate\n" \
"Location: /uporabniki/%s/\n" \
"Content-Type: text/plain\n" \
"\n" \
"Document relocated\n" \

failed = \
"Status: 302 relocate\n" \
"Location: /napaka/401.html\n" \
"Content-Type: text/plain\n" \
"\n" \
"Document relocated\n" \

def HandleRequest(form):
    user = os.environ['REMOTE_USER']
##     if Formater_Client.formater_client.open()>=0:
## 	Formater_Client.formater_client.send("MakeUser %s" % user)
## 	# Get response, but ignore it:
## 	Formater_Client.formater_client.response()
#    try:
    oid = Uporabniki.UserID(user)
    print rel_msg % oid
#    except Uporabniki.NoSuchUser:
#	print failed
