# $ProjectHeader: volitve 0.7 Mon, 08 Sep 1997 17:37:41 +0200 andrej $
#
# $Id: MakeDodaj.py 1.2 Fri, 05 Sep 1997 14:43:33 +0000 andrej $
# Naredi dokument dodaj.html: zares opravi delo.

import pg95
import admin_cfg

def run(srcdir, destdir, templates):
    pg95.set_defbase(admin_cfg.DB_Name)
    conn = pg95.connect()
    db_papirji = conn.query("SELECT papir_id FROM papirji ORDER BY papir_id")
    Papirji = ""
    for p in db_papirji:
	Papirji = Papirji + "<OPTION>%s\n" % p
    #    print "Papirji:\n", Papirji
    DolzinaPapirjev=len(db_papirji)

    db_stranke = conn.query("SELECT stranka_id FROM stranke ORDER BY stranka_id")
    Ponudniki = ""
    for s in db_stranke:
	Ponudniki = Ponudniki + "<OPTION>%s\n" % s
#    print "Ponudniki:\n", Ponudniki

    # Preberi obrazec:
    templname = srcdir + '/' + templates['Dodaj']['ime'] + '.in'

    f = open(templname, "r")
    content = f.read()
    f.close();

#    print content

#    print locals()

#    print content % locals()
    
    destname = destdir + templates['Dodaj']['dir'] + '/' + \
		templates['Dodaj']['ime'] 
    g = open(destname, "w")
    g.write(content % locals())
    g.close


