# $ProjectHeader: volitve 0.14 Thu, 25 Sep 1997 21:32:05 +0200 andrej $
#
# $Id: Uporabniki.py 1.4 Wed, 24 Sep 1997 17:03:46 +0000 andrej $
#
# Definira preslikavo iz uporabni¹kega imena v ID. Pa ¹e kaj?

import Admin_Client
import cgi_config
import string

# Tole je globalno: med posameznimi klici se povezava ohranja 
# To bi seveda moralo biti hitreje...

_USERMAP = {}

# Povsem podobna funkcija je v Registratorju v administraciji.
# Tale v bistvu klièe tisto funkcijo.
def UserID(user):
    global _USERMAP

    if _USERMAP.has_key(user):
	result = _USERMAP[user]
    else:
	if Admin_Client.admin_client.open()<0:
	    raise "Administrator down"
	Admin_Client.admin_client.send('UserID ' + user)
	result = Admin_Client.admin_client.response()

	if result[:3]=="100":
	    raise "No such user: " + user
	result = result[4:]
	_USERMAP[user] = result
    return result

# test    
if __name__ == '__main__':
    print UserID('CRT')
    print UserID('No Such User')
