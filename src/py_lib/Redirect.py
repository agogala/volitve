# $ProjectHeader: volitve 0.18 Sun, 05 Oct 1997 22:53:12 +0200 andrej $
#
# $Id: Redirect.py 1.1 Wed, 10 Sep 1997 16:15:50 +0000 andrej $
#
# Preusmeri uporabnika v njegov direktorij.

import Uporabniki
import os

rel_msg = \
"Status: 302 relocate\n" \
"Location: /uporabniki/%s\n" \
"Content-Type: text/plain\n" \
"\n" \
"Document relocated\n" \


def HandleRequest(form):
    user = os.environ['REMOTE_USER']
    oid = Uporabniki.UserID(user)
    print rel_msg % oid
