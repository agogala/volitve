# $ProjectHeader: volitve 0.10 Thu, 11 Sep 1997 18:28:32 +0200 andrej $
#
# $Id: Uporabniki.py 1.2 Thu, 11 Sep 1997 16:28:32 +0000 andrej $
#
# Definira preslikavo iz uporabni¹kega imena v ID. Pa ¹e kaj?

import Admin_Client
import cgi_config

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
	Admin_Client.admin_client.send('U' + user)
	result = Admin_Client.admin_client.response()

	if result=="-1":
	    raise "No such user: " + user
	_USERMAP[user] = result
    return result
    
if __name__ == '__main__':
    print UserID('CRT')
    print UserID('No Such User')
