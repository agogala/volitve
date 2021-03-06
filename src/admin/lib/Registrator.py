# $ProjectHeader: volitve 0.28 Sat, 27 Dec 1997 16:06:49 +0100 andrej $
#
# $Id: Registrator.py 1.10 Fri, 21 Nov 1997 17:06:57 +0000 andrej $
# Se ukvarja z registracijo uporabnikov

import pg95
import admin_cfg
import sys
import os
import Util
import AdminConst
import DBConn
import MakePregled

# Imamo kar stalno povezavo z bazo:
db_conn = DBConn.db_conn

# Preveri, ?e je hash v bazi:
def Validate(hash):
    if admin_cfg.MarketClosed():
	return AdminConst.Register.MarketClosed
    gethash = db_conn.query(
	"SELECT email, hash, accessed FROM registracije WHERE hash='%s'" %\
	hash)
    # Nismo ?e v bazi:
    if not gethash:
	return AdminConst.Validate.NoHash
    # upsa!, ve? enakih, interna napaka:
    if len(gethash)!=1:
	raise "Too many hashes in base"
    # Preverimo, ?e se je ?e povezal.
    accessed = gethash[0][2]
    if accessed=='t':
	return AdminConst.Validate.HashAccessed

    return AdminConst.Validate.OK

# Vrni ID za danega uporabnika
# Podobna funkija je v Uporabniki v cgi direktoriju...
# Cache imen: tako bo delalo hitreje
_USERMAP = {}

def UserID(user):
    global _USERMAP

    if _USERMAP.has_key(user):
	result = _USERMAP[user]
    else:
	list = db_conn.query("SELECT oid FROM stranke WHERE stranka_id='%s'" % user[:10])
	if len(list)==0:
	    return (AdminConst.UserID.NoUser, "")
	result = list[0][0]
	_USERMAP[user] = result
    return (AdminConst.UserID.OK, result)

# Registracija uporabnikov:
# Gesla: tole sem pobral iz htpasswd.c
itoa64 = "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"

def to64(v, n):
    result = ''
    for i in range(n):
	result = result + itoa64[v & 0x3f]
	v = v >> 6
    return result

def EncryptPasswd(passwd):
    import time
    import rand
    rand.srand(int(time.time()))
    salt = to64(rand.rand(), 2)
    import crypt
    return crypt.crypt(passwd, salt)

def Registriraj(hash, username, passwd):
    if admin_cfg.MarketClosed():
	return AdminConst.Register.MarketClosed
    # Poi??i hash:
    gethash = db_conn.query(
	"SELECT email, hash, accessed FROM registracije WHERE hash='%s'" %\
	hash) 
    if not gethash:
	return AdminConst.Register.NoHash
    if len(gethash)>1:
	raise "Too many hashes in base"
    from RegisterReply import NormalizeAddress
    email = NormalizeAddress(gethash[0][0])
    # Dodaj v bazo:
    # Preveri, ?e ?e obstaja v bazi:
    getemail = db_conn.query("SELECT email FROM stranke WHERE email='%s'" %\
			     email)
    if getemail:
	return AdminConst.Register.DuplicateEmail
    # Preverimo, ?e je ime ?e porabljeno:
    getusername = db_conn.query(
	"SELECT stranka_id FROM stranke WHERE stranka_id='%s'" % username)
    if getusername:
	return AdminConst.Register.DuplicateUser
    try:
	db_conn.query("BEGIN")
	# Dodaj stranko:
	db_conn.query(
	    "INSERT INTO stranke (stranka_id, email) VALUES('%s','%s')" % \
	    (username, email))
	# Dodaj stanje:
	db_conn.query(
	    "INSERT INTO stanje (stranka_id, papir_id, kolicina) SELECT '%s' \
AS Stranka_ID, Papir_ID, 0 AS kolicina FROM Papirji" % username)
	db_conn.query(
	    "INSERT INTO stanje (stranka_id, papir_id, kolicina) \
VALUES('%s','denar',0)" % username)

        # Ozna?imo spremembo
	db_conn.query("UPDATE registracije SET accessed='t' WHERE hash='%s'" % hash)
	# Ustvari home direktorij:
	rc, ID = UserID(username)
	userroot = admin_cfg.htmldir + \
		   admin_cfg.templates['Stanje']['dir'] 
	userdir = userroot + '/' + ID
	os.mkdir(userdir)

	templname = admin_cfg.tempdir + '/' + \
		    admin_cfg.templates['HTAccess']['ime'] + '.in'
	destname = userdir + '/.htaccess'
	Util.MakeTemplate(templname, destname, {'username': username})

	# Dodaj password v htpasswd:
	cpasswd = EncryptPasswd(passwd)
	fname = admin_cfg.PasswdFile
	import tempfile
	tmpname = tempfile.mktemp()
	outf = open(tmpname, "w")
	if not outf:
	    raise "Couldn't open tempfile"
	inf = open(fname, "r")
	if not inf:
	    raise "Couldn' open passwd file"
	found = 0
	l = len(username)
	while 1:
	    line = inf.readline()
	    if not line:
		break
	    if line[0]=='#':
		outf.write(line)
		continue
	    if line[:l]==username and line[l]==':':
		if not found:
		    # Na?el uporabnika:
		    outf.write('%s:%s\n' % (username, cpasswd))
		    found = 1		
	    else:
		# Prekopiramo vrstico:
		outf.write(line)

	if not found:
	    outf.write('%s:%s\n' % (username, cpasswd))
	
	outf.close()
	inf.close()
	# Naredimo kopijo:
	os.system('cp %s %s' % (tmpname, fname))
	os.unlink(tmpname)

	db_conn.query("COMMIT")
    except:
	db_conn.query("ROLLBACK")
	ety = sys.exc_type
	evl = sys.exc_value
	etr = sys.exc_traceback
	try:
	    os.unlink(tmpname)
	except:
	    pass
	try:
	    del _USERDIR[user]
	except:
	    pass
	raise ety, evl, etr

    # ?e je vse v redu, naredimo ?e pregled: 
    # Le ta namre? izra?una trenutne poravnalne cene in zato 
    # uporablja transakcije...
    MakePregled.updateuser(admin_cfg.tempdir, userroot, 
			   admin_cfg.templates, username)


    return AdminConst.Register.OK
