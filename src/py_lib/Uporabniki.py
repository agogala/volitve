# $ProjectHeader: volitve 0.9 Wed, 10 Sep 1997 18:15:50 +0200 andrej $
#
# $Id: Uporabniki.py 1.1 Wed, 10 Sep 1997 16:15:50 +0000 andrej $
#
# Definira preslikavo iz uporabni¹kega imena v ID. Pa ¹e kaj?

import pg95
import cgi_config

# Tole je globalno: med posameznimi klici se povezava ohranja 
# To bi seveda moralo biti hitreje...
pg95.set_defbase(cgi_config.DB_Name)
db_conn = pg95.connect()

_USERMAP = {}

def UserID(user):
    global _USERMAP

    if _USERMAP.has_key(user):
	result = _USERMAP[user]
    else:
	list = db_conn.query("SELECT oid FROM stranke WHERE stranka_id='%s'" % user)
	if len(list)==0:
	    raise "No such user"
	result = list[0][0]
	_USERMAP[user] = result
    return result
    
#if __name__ == '__main__':
#    print UserID('CRT')
