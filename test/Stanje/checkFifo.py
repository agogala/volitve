#!/net/rozle/export/share/sol/bin/python

import pg95
import os

pg95.set_defbase("zupani")
conn = pg95.connect()

db_pap = conn.query("Select papir_id from papirji")
rez = {}
for pap in db_pap:
    db_ponudbe = conn.query("SELECT min(cena) as cena, ponudnik, kolicina, datum, ura FROM fifo WHERE kolicina<0 and papir_id='%s' GROUP BY ponudnik, kolicina, datum, ura ORDER BY cena, datum, ura" % pap[0])
    if len(db_ponudbe)>0:
	if not rez.has_key(db_ponudbe[0][1]):
	    rez[db_ponudbe[0][1]] = {}
	rez[db_ponudbe[0][1]][pap[0]+'n'] = db_ponudbe[0]
    else:
	print "Ni ponudbe"

    db_povpr = conn.query("SELECT max(cena) as cena, ponudnik, kolicina, datum, ura FROM fifo WHERE kolicina>0 and papir_id='%s' GROUP BY ponudnik, kolicina, datum, ura ORDER BY cena desc, datum, ura" % pap[0])
    if len(db_povpr)>0:
	if not rez.has_key(db_povpr[0][1]):
	    rez[db_povpr[0][1]] = {}
	rez[db_povpr[0][1]][pap[0]+'p'] = db_povpr[0]
    else:
	print "Ni povpra¹evanj"

rkey = rez.keys()
rkey.sort()
for k in rkey:
    pkey = rez[k].keys()
    pkey.sort()
    for p in pkey:
	print "%s: %s %3s po %5s" % (rez[k][p][1], p, rez[k][p][2], rez[k][p][0])
	os.system("./testS %s %s" %(rez[k][p][1], p))
	
  
