# $ProjectHeader: volitve 0.27 Fri, 21 Nov 1997 18:06:57 +0100 andrej $
#
# $Id: Uporabniki.py 1.5 Mon, 03 Nov 1997 13:25:50 +0000 andrej $
#
# Definira preslikavo iz uporabni¹kega imena v ID. Pa ¹e kaj?

import Admin_Client
import cgi_config
import string

# Tole je globalno: med posameznimi klici se povezava ohranja 
# To bi seveda moralo biti hitreje...

_USERMAP = {}

NoSuchUser = "NoSuchUser"

# Povsem podobna funkcija je v Registratorju v administraciji.
# Tale v bistvu klièe tisto funkcijo.
def UserID(user):
    global _USERMAP

    if _USERMAP.has_key(user):
	result = _USERMAP[user]
    else:
	if Admin_Client.admin_client.open()<0:
	    raise "Administrator down"
	Admin_Client.admin_client.send('UserID "%s"' % user)
	result = Admin_Client.admin_client.response()

	if result[:3]=="100":
	    raise "NoSuchUser"
	result = result[4:]
	_USERMAP[user] = result
    return result

# test    
if __name__ == '__main__':
    print UserID('CRT')
    print UserID('No Such User')
