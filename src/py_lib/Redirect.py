# $ProjectHeader: volitve 0.22 Sun, 26 Oct 1997 22:47:33 +0100 andrej $
#
# $Id: Redirect.py 1.2 Thu, 09 Oct 1997 13:19:34 +0000 andrej $
#
# Preusmeri uporabnika v njegov direktorij.

import Uporabniki
import os

rel_msg = \
"Status: 302 relocate\n" \
"Location: /uporabniki/%s/\n" \
"Content-Type: text/plain\n" \
"\n" \
"Document relocated\n" \


def HandleRequest(form):
    user = os.environ['REMOTE_USER']
    oid = Uporabniki.UserID(user)
    print rel_msg % oid
