# $ProjectHeader: volitve 0.15 Fri, 26 Sep 1997 18:28:00 +0200 andrej $
#
# $Id: MakePregled.py 1.5 Fri, 26 Sep 1997 16:28:00 +0000 andrej $
# Naredi dokument pregled.html: zares opravi delo.

import pg95
import admin_cfg
import Util
import DBConn
import Registrator

# Imamo kar stalno povezavo z bazo:
conn = DBConn.db_conn

def CollectCols(list):
    val = ""
    for l in list:
	val = val + "<TD>" + l + "\n"
    return val
	
def run(srcdir, destdir, templates):
    db_fifo = conn.query("SELECT * FROM fifo ORDER BY papir_id, cena, datum, ura ")
    FIFO = "<tr>" + CollectCols(conn.listfields()) + "</tr>\n"
    for f in db_fifo:
	FIFO = FIFO + "<tr>" + CollectCols(f) + "</tr>\n"
#    print "Fifo:\n", FIFO

    db_posli = conn.query(
	"SELECT * FROM posli ORDER BY datum using >, ura using >")
    Posli = "<tr>" + CollectCols(conn.listfields()) + "</tr>\n"
    for p in db_posli[:30]:
	Posli = Posli + "<tr>" + CollectCols(p) + "</tr>\n"
#    print "Posli:\n", Posli

    # Preberi obrazec:
    templname = srcdir + '/' + templates['Pregled']['ime'] + '.in'
    destname = destdir + templates['Pregled']['dir'] + '/' + \
		templates['Pregled']['ime'] 
    Util.MakeTemplate(templname, destname, locals())


def updateuser(srcdir, destdir, templates, user):
    usr_fifo = conn.query("SELECT * FROM fifo WHERE ponudnik='%s' ORDER BY papir_id, cena, datum, ura" % user)
    FIFO = "<tr>" + CollectCols(conn.listfields()) + "</tr>\n"
    for f in usr_fifo:
	FIFO = FIFO + "<tr>" + CollectCols(f) + "</tr>\n"
    
    usr_stanje = conn.query("SELECT * FROM stanje WHERE stranka_id='%s' ORDER BY papir_id" % user)
    Papirji = []
    Kolicine = []
    for f in usr_stanje:
	Papirji.append(f[1])
	Kolicine.append(f[2])
    Stanje = "<tr>" + CollectCols(Papirji) + "</tr>\n"
    Stanje = Stanje + "<tr>" + CollectCols(Kolicine) + "</tr>\n"


##     Stanje = "<tr>" + CollectCols(conn.listfields()) + "</tr>\n"
##     for f in usr_stanje:
## 	Stanje = Stanje + "<tr>" + CollectCols(f) + "</tr>\n"

    (result, userid) = Registrator.UserID(user)
    if result!=0: # Error selecting user!
	return

    # Preberi obrazec:
    templname = srcdir + '/' + \
		admin_cfg.templates['Stanje']['ime'] + '.in'
    destname = destdir + '/' + userid + '/index.html'
    Util.MakeTemplate(templname, destname, {'username': user,
					    'FIFO': FIFO,
					    'Stanje': Stanje})
    
	
def defupdateuser(user):
    userroot = admin_cfg.htmldir + \
	       admin_cfg.templates['Stanje']['dir'] 
    updateuser(admin_cfg.tempdir, userroot,
	       admin_cfg.templates, user)
