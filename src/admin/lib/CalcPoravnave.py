# $ProjectHeader: volitve 0.28 Sat, 27 Dec 1997 16:06:49 +0100 andrej $
#
# $Id: CalcPoravnave.py 1.1 Sat, 27 Dec 1997 15:06:49 +0000 andrej $
#
# Izraèunaj poravnave.

import pg95
import admin_cfg
import Util
import DBConn
import string

conn = DBConn.db_conn

def run():
    conn.query("DELETE FROM rezultati")
    db_por = conn.query("SELECT * FROM poravnave");
    por = {}
    for i in db_por:
	por[i[0]] = string.atof(i[1])

    db_stranke = conn.query("SELECT stranka_id FROM stranke");
    for s in db_stranke:
	user = s[0]
	usr_stanje = conn.query("SELECT * FROM stanje WHERE stranka_id='%s' ORDER BY papir_id" % user)
	rezultat = 0
	for f in usr_stanje:
	    if string.strip(f[1])<>"denar":
		rezultat = rezultat + string.atof(f[2]) * por[f[1]]
	    else: # Denar:
		rezultat = rezultat + string.atof(f[2])
	conn.query("INSERT INTO rezultati VALUES ('%s', %f)" % (user, rezultat))
    
#     keys = rez.keys()
#     keys.sort()
#     for k in keys:
# 	print "%s: %5.2f" % (k, rez[k])
