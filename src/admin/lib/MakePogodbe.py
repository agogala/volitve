# $ProjectHeader: volitve 0.26 Sat, 08 Nov 1997 08:02:11 +0100 andrej $
#
# $Id: MakePogodbe.py 1.1 Sat, 08 Nov 1997 07:02:11 +0000 andrej $
# Naredi pregled pogodb za posamezno stranko.

import pg95
import admin_cfg
import Util
import DBConn
import Registrator
import time
import os
import string

conn = DBConn.db_conn

def run(srcdir, destdir, templates, user, paper):
    # Izraèunamo ime datoteke:
    (result, userid) = Registrator.UserID(user)
    if result!=0: # Error selecting user!
	return
    destname = destdir + '/' + userid + '/' + paper + '.html'

## Èasa ne preverjamo, ker zaenkrat ne znam hitro izracunati casa zadnje pogodbe v postgresu. Probal sem max(datetime(datum,ura)), pa se sesuje!!!
##    stat = os.stat(destname)
    papir_id = conn.query("select papir_id from papirji where papir_id='%s'" % paper)
    if len(papir_id)==0:
	return

    pogodbe = conn.query((
	"select datum,ura,cena,kolicina,prodajalec,kupec from posli\n" + 
	"where papir_id='%(paper)s' and\n" +
	"(prodajalec='%(user)s' or kupec='%(user)s')\n" +
	"order by datum desc, ura desc") % {'user':user, 'paper':paper})
    
    tb = '<thead><tr><th>Datum<th>Ura<th>Cena<th>Kolièina\n<tbody>'
    for p in pogodbe:
	if p[4]==user:
	    kolicina='-' + p[3]
	else:
	    kolicina=p[3]
	tb = tb + '<tr><td>%(datum)s<td>%(ura)s<td align="right">%(cena)s<td align="right">%(kolicina)s\n' % \
	     {'datum': Util.RewriteDate(p[0]),
	      'ura': p[1],
	      'cena': Util.FormatFloat(string.atof(p[2])),
	      'kolicina': kolicina}

    t = time.localtime(time.time())
    datum = time.strftime('%d.%m.%y',t)
    ura = time.strftime('%H:%M',t)
    
    # Preberi obrazec:
    templname = srcdir + '/' + \
		admin_cfg.templates['Pogodbe']['ime'] + '.in'
    Util.MakeTemplate(templname, destname, {'username': user,
					    'paper': paper,
					    'Pogodbe': tb,
					    'Datum': datum,
					    'Ura': ura})

def defaultrun(user, paper):
    userroot = admin_cfg.htmldir + \
	       admin_cfg.templates['Pogodbe']['dir'] 
    run(admin_cfg.tempdir, userroot,
	       admin_cfg.templates, user, paper)
    
