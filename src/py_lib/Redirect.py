# $ProjectHeader: volitve 0.21 Tue, 21 Oct 1997 21:26:41 +0200 andrej $
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
