# $ProjectHeader: volitve 0.27 Fri, 21 Nov 1997 18:06:57 +0100 andrej $
#
# $Id: MakePogodbe.py 1.2 Fri, 21 Nov 1997 17:06:57 +0000 andrej $
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

    papir_id = conn.query("select papir_id from papirji where papir_id='%s'" % paper)
    if len(papir_id)==0:
	return

    pogodbe = conn.query((
	"select datum,ura,cena,kolicina,prodajalec,kupec from posli\n" + 
	"where papir_id='%(paper)s' and\n" +
	"(prodajalec='%(user)s' or kupec='%(user)s')\n" +
	"order by datum desc, ura desc") % {'user':user, 'paper':paper})
    
    tb = '<thead><tr><th>Datum<th>Ura<th>Cena<th>Kolièina\n<tbody>'
    prod_p = 0
    prod_n = 0
    sum_p = 0
    sum_n = 0
    for p in pogodbe:	
	cena = string.atof(p[2])
	kolicina = string.atoi(p[3])
	if string.strip(p[4])==user:
	    # Prodaja
	    prod_p = prod_p + kolicina * cena
	    sum_p = sum_p + kolicina
	    kolicina_s='-' + p[3]
	else:
	    # Nakup
	    prod_n = prod_n + kolicina * cena
	    sum_n = sum_n + kolicina
	    kolicina_s=p[3]
	tb = tb + '<tr><td>%(datum)s<td>%(ura)s<td align="right">%(cena)s<td align="right">%(kolicina)s\n' % \
	     {'datum': Util.RewriteDate(p[0]),
	      'ura': p[1],
	      'cena': Util.FormatFloat(cena),
	      'kolicina': kolicina_s}

    try:
	prodaje = prod_p / sum_p
    except:
	prodaje = 0
    try:
	nakupi = prod_n / sum_n
    except:
	nakupi = 0

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
					    'Ura': ura,
					    'nakupi': Util.FormatFloat(nakupi),
					    'prodaje': Util.FormatFloat(prodaje)})

def defaultrun(user, paper):
    userroot = admin_cfg.htmldir + \
	       admin_cfg.templates['Pogodbe']['dir'] 
    run(admin_cfg.tempdir, userroot,
	       admin_cfg.templates, user, paper)
    
