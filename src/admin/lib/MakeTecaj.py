# $ProjectHeader: volitve 0.20 Sun, 19 Oct 1997 19:07:54 +0200 andrej $
#
# $Id: MakeTecaj.py 1.1 Sun, 19 Oct 1997 17:07:54 +0000 andrej $
#
# Pripravi teèajnico.


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

def run(srcdir, destdir, templates, dan='yesterday'):
    # Izracunaj tecaje za dani dan:
    db_tecaji = conn.query((
	"SELECT '%(dan)s' AS Datum, Papir_id,\n" + 
	"sum(float8(kolicina) * cena) / float8(sum(kolicina)) AS tecaj,\n" +
	"sum(kolicina) AS promet\n" +
	"FROM posli\n" +
	"WHERE datum='%(dan)s' and kupec!=prodajalec\n" +
	"GROUP BY papir_id") % {'dan': dan})

    db_pap = conn.query("SELECT papir_id FROM papirji ORDER BY papir_id")
    print db_tecaji
    print db_pap

def Old():
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

    
