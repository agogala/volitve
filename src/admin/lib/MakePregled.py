# $ProjectHeader: volitve 0.8 Tue, 09 Sep 1997 00:58:50 -2200 andrej $
#
# $Id: MakePregled.py 1.3 Mon, 08 Sep 1997 15:37:41 +0000 andrej $
# Naredi dokument pregled.html: zares opravi delo.

import pg95
import admin_cfg

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

    f = open(templname, "r")
    content = f.read()
    f.close();

#    print content

#    print locals()

#    print content % locals()
    
    destname = destdir + templates['Pregled']['dir'] + '/' + \
		templates['Pregled']['ime'] 
    g = open(destname, "w")
    g.write(content % locals())
    g.close


