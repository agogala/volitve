# $ProjectHeader: volitve 0.20 Sun, 19 Oct 1997 19:07:54 +0200 andrej $
#
# $Id: MakePregled.py 1.6 Thu, 09 Oct 1997 13:19:34 +0000 andrej $
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
    db_ponudbe = conn.query("SELECT max(cena), papir_id FROM fifo WHERE kolicina<0 GROUP BY papir_id ORDER BY papir_id")
    ponudbe = {}
    for p in db_ponudbe:
	ponudbe[p[1]] = p[0]

    db_povpr = conn.query("SELECT min(cena), papir_id FROM fifo WHERE kolicina>0 GROUP BY papir_id ORDER BY papir_id")
    povpr = {}
    for p in db_povpr:
	povpr[p[1]] = p[0]

    db_pap = conn.query("SELECT papir_id FROM papirji ORDER BY papir_id")
    FIFO = "<tr><td><td>Ponudba<td>Povpra¹evanje</tr>\n<tr>\n"
#    for p in db_pap:
#	FIFO = FIFO + '<td colspan="2">' + p[0] + "\n"
#    FIFO = FIFO + "</tr>\n<tr>\n"

#    for p in db_pap:
#	FIFO = FIFO + "<td>Ponudba\n <td>Povpra¹evanje\n" 

#    FIFO = FIFO + "</tr>\n<tr>\n"
    for p in db_pap:
	if ponudbe.has_key(p[0]):
	    pon = ponudbe[p[0]]
	else:
	    pon = ""
	if povpr.has_key(p[0]):
	    pov = povpr[p[0]]
	else:
	    pov = ""
	FIFO = FIFO + '<tr>\n<td>%s\n<td align="right">%s\n<td align="right">%s\n</tr>\n' % (p[0], pon, pov)

    # Preberi obrazec:
    templname = srcdir + '/' + templates['Pregled']['ime'] + '.in'
    destname = destdir + templates['Pregled']['dir'] + '/' + \
		templates['Pregled']['ime'] 
    Util.MakeTemplate(templname, destname, locals())

def RewriteDate(d):
    return '%s.%s.%s' % (d[3:5], d[0:2], d[8:10])

def MakeUserRow(f):
    val = "<tr>\n"
    # OID:
    val = val + '<td><input type="checkbox" name="%s">\n' % f[0]
    #Papir:
    val = val + '<td>\n' + f[1]
    # Cena
    val = val + '<td align="right">\n' + f[2]
    # Kolièina
    val = val + '<td align="right">\n' + f[3]
    # Datum
    val = val + '<td>\n' + RewriteDate(f[4])
    # Ura
    val = val + '<td>\n' + f[5]
    val = val + "</tr>\n"
    return val

def updateuser(srcdir, destdir, templates, user):
    usr_ponudbe = conn.query("SELECT oid, papir_id, cena, -kolicina, datum, ura FROM fifo WHERE ponudnik='%s' AND kolicina<0 ORDER BY papir_id, cena, datum, ura" % user)

    if len(usr_ponudbe)>0:
	FIFO = 'Ponudbe:\n<table cellspacing="1" bgcolor="lightblue" border="1">\n'
	FIFO = FIFO + "<tr><td>Preklièi<td>Papir<td>Cena<td>Kolièina<td>Datum<td>Ura</tr>\n"
	for f in usr_ponudbe:
	    FIFO = FIFO + MakeUserRow(f)
	FIFO = FIFO + '</table><br>'
    else:
	FIFO = ""

    usr_povpr = conn.query("SELECT oid, papir_id, cena, kolicina, datum, ura FROM fifo WHERE ponudnik='%s' AND kolicina>0 ORDER BY papir_id, cena, datum, ura" % user)
    
    if len(usr_povpr)>0:
	FIFO = FIFO + 'Povpra¹evanje:\n<table cellspacing="1" bgcolor="lightblue" border="1">\n'
	FIFO = FIFO + "<tr><td>Preklièi<td>Papir<td>Cena<td>Kolièina<td>Datum<td>Ura</tr>\n"
	for f in usr_povpr:
	    FIFO = FIFO + MakeUserRow(f)
	FIFO = FIFO + "</table>\n<br>"

    if FIFO!="":
	FIFO = FIFO + '<input type="submit" name="Zbrisi" value="  Preklic  ">'

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
