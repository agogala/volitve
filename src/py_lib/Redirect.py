# $ProjectHeader: volitve 0.23 Tue, 28 Oct 1997 21:15:29 +0100 andrej $
#
# $Id: Redirect.py 1.3 Tue, 28 Oct 1997 20:15:29 +0000 andrej $
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


def HandleRequest(form):
    user = os.environ['REMOTE_USER']
##     if Formater_Client.formater_client.open()>=0:
## 	Formater_Client.formater_client.send("MakeUser %s" % user)
## 	# Get response, but ignore it:
## 	Formater_Client.formater_client.response()
    oid = Uporabniki.UserID(user)
    print rel_msg % oid
