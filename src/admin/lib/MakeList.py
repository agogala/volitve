# $ProjectHeader: volitve 0.28 Sat, 27 Dec 1997 16:06:49 +0100 andrej $
#
# $Id: MakeList.py 1.1 Sat, 27 Dec 1997 15:06:49 +0000 andrej $
#
# Sestavi seznam udele¾encev

import pg95
import admin_cfg
import Util
import DBConn
import string

conn = DBConn.db_conn

def run():
    db_rez = conn.query("SELECT * FROM rezultati ORDER BY rezultat desc")
    list = ''
    neposli = ''
    num = 1
    for r in db_rez:
	rez = string.atof(r[1])
	if rez==0:
	    db_posli = conn.query("SELECT * FROM posli WHERE kupec='%s' or prodajalec = '%s'" % (r[0],r[0]))
	    if len(db_posli)==0:
		neposli = neposli + "<li>" + r[0] + "\n"
		continue
	list = list +  '<tr><td>%d.<td align="Left">%15s<td align="Right"> %s\n' % (num, r[0], Util.FormatFloat(rez))
	num = num + 1
    # Tu bi moral prebrati template itd. Tole je bolj enostavno :-)
    print list
    print
    print neposli
