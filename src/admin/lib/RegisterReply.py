# $ProjectHeader: volitve 0.14 Thu, 25 Sep 1997 21:32:05 +0200 andrej $
#
# $Id: RegisterReply.py 1.4 Mon, 22 Sep 1997 13:21:03 +0000 andrej $
# Pripravi vse za registracijo - direktorij + odgovor

import rfc822
import sys
import pg95
import admin_cfg
import os

UserError = 'User_Error'

def NormalizeAddress(addr):
    import string
    return string.lower(addr)

def byte2string(instr):
    result = ''
    for l in range(len(instr)):
	result = result + ('%02x' % ord(instr[l]))
    return result
    
def MakeUrl(name):
    import md5, time
    md5hash = md5.new()
    md5hash.update(name)
    md5hash.update(time.asctime(time.gmtime(time.time())))
    result = "%s" % byte2string(md5hash.digest())
    return result

def run(srcdir, templates):
    message = rfc822.Message(sys.stdin)
    if message.has_key('Reply-To'):
	ekey = 'Reply-To'
    elif message.has_key('From'):
	ekey = 'From'
    else:
	raise UserError, "No Reply-To: or From: header"

    replyto = message.getheader(ekey)
    name, email = message.getaddr(ekey)
    email = NormalizeAddress(email)
    
    pg95.set_defbase(admin_cfg.DB_Name)
    conn = pg95.connect()
    exists = conn.query("SELECT * FROM stranke WHERE EMail='%s'" % email)
    if exists:
	filekey = 'RegisterOld'
    else:
	filekey = 'RegisterNew'

    # Preveri, èe ¾e obstaja tak e-mail v ¹e ne izkori¹èenih registracijah
    rereg = conn.query("SELECT hash FROM Registracije WHERE EMail='%s'" % email)
    
    if rereg:
	urlkey = rereg[0][0]
    else:
	urlkey = MakeUrl(replyto)
	conn.query(
	    "INSERT INTO Registracije (EMail, hash, accessed) " + 
	    "VALUES ('%s', '%s', 'f')" % \
	    (email, urlkey))

    url = 'http://rozle.e5.ijs.si:4242/registriraj/' + urlkey

    # Preberi obrazec:
    templname = srcdir + '/' + templates[filekey]['ime'] 

    f = open(templname, "r")
    content = f.read()
    f.close();

    message = content % {
	'replyto': replyto,
	'url': url,
	'serveradmin': 'andrej@e5.ijs.si'
	}
    pipe = os.popen(admin_cfg.SENDMAIL, "w")
    pipe.write(message)
    pipe.close()

if __name__ == '__main__':
    print MakeUrl('andrej') 
