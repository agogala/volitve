# $ProjectHeader: volitve 0.12 Mon, 22 Sep 1997 15:21:03 +0200 andrej $
#
# $Id: Reg_Client.py 1.1 Mon, 22 Sep 1997 13:21:03 +0000 andrej $
#
# Registriraj uporabnika:

import Admin_Client

def HandleForm():
    if Admin_Client.admin_client.open()<0:
	return "<H1>Napaka</H1>\n" + \
	       "Administrator je zaprt\n"
	
    fields = ["username", "passwd", "passwd1"]
    ...
