# $ProjectHeader: volitve 0.10 Thu, 11 Sep 1997 18:28:32 +0200 andrej $
#
# $Id: Registrator.py 1.1 Thu, 11 Sep 1997 16:28:32 +0000 andrej $
# Se ukvarja z registracijo uporabnikov

import pg95
import admin_cfg

# Imamo kar stalno povezavo z bazo:
pg95.set_defbase(admin_cfg.DB_Name)
db_conn = pg95.connect()

# Preveri, èe je hash v bazi:
def Validate(hash):
    gethash = db_conn.query(
	"SELECT email, hash, accessed FROM registracije WHERE hash='%s'" %\
	hash)
    # Nismo ¹e v bazi:
    if len(gethash)==0:
	return -1
    # upsa!, veè enakih, interna napaka:
    if len(gethash)!=1:
	raise "Too many hashes in base"
    # Preverimo, èe se je ¾e povezal.
    accessed = gethash[0][3]
    if accessed=='t':
	return -1

    # Oznaèimo spremembo
    db_conn.query("UPDATE registracije SET accessed='t'")
    return 0

def Registriraj(hash, username, passwd):
    # Poi¹èi hash:
    gethash = db_conn.query(
	"SELECT email, hash, accessed FROM registracije WHERE hash='%s'" %\
	hash) 
    # Dodaj v bazo:
    #[...]
    # Ustvari home direktorij:

    return 0

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
