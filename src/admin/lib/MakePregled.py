# $ProjectHeader: volitve 0.12 Mon, 22 Sep 1997 15:21:03 +0200 andrej $
#
# $Id: MakePregled.py 1.4 Thu, 11 Sep 1997 21:18:12 +0000 andrej $
# Naredi dokument pregled.html: zares opravi delo.

import pg95
import admin_cfg
import Util

# Imamo kar stalno povezavo z bazo:
pg95.set_defbase(admin_cfg.DB_Name)
conn = pg95.connect()

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


