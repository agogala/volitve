# $ProjectHeader: volitve 0.26 Sat, 08 Nov 1997 08:02:11 +0100 andrej $
#
# $Id: MakeTecaj.py 1.7 Sat, 08 Nov 1997 07:02:11 +0000 andrej $
#
# Pripravi teèajnico.

import pg95
import admin_cfg
import Util
import DBConn
import Registrator
import string
import os
import sys

# Imamo kar stalno povezavo z bazo:
conn = DBConn.db_conn

# Cache teèajev:
_TECAJIMAP = {}

def IzracunajTecaj(dan='yesterday'):
    conn.query("begin")
    try:
	# Izracunaj tecaje za dani dan:
	db_tecaji = conn.query((
	    "SELECT '%(dan)s'::date,\n" +
	    "Papir_id,\n" + 
	    "sum(float8(kolicina) * cena) / float8(sum(kolicina)) AS tecaj,\n" +
	    "min(cena) as MinCena,\n" + 
	    "max(cena) as MaxCena,\n" +
	    "count(kolicina) AS obseg,\n" +
	    "sum(kolicina) AS promet\n" +
	    "FROM posli\n" +
	    "WHERE datum='%(dan)s' and kupec!=prodajalec\n" +
	    "GROUP BY papir_id") % {'dan': dan})
	vstavil = 0
	# Pobri¹i prej¹nje teèaje:
	conn.query("DELETE FROM Tecajnica WHERE datum='%s'" % dan)
	db_manjkajo = conn.query("SELECT Papir_id FROM Papirji")
	for f in db_tecaji:
	    if f[1]<>"":
		conn.query(
		    "INSERT INTO Tecajnica\n" + 
		    "VALUES('%s', '%s', %s, %s, %s, %s, %s)" % f)
		db_manjkajo.remove((f[1],))
		vstavil = 1
	# Izraèunamo ¹e preostale teèaje:
	if len(db_manjkajo)>0:
	    manjkajo = '('
	    for p in db_manjkajo:
		manjkajo = manjkajo + "'%s'" % p[0] + ','
	    manjkajo = manjkajo[:-1] + ')'
	    stari_tecaji = conn.query((
		"SELECT max(datum) as datum,\n" +
		"papir_id\n" +
		"FROM Tecajnica\n" +
		"WHERE datum<'%(dan)s' and\n" + 
		"papir_id in " + manjkajo + "\n" +
		"GROUP BY papir_id\n" +
		"ORDER BY papir_id") % {'dan':dan})
	
	    for st in stari_tecaji:
		if st[0]<>'':
		    conn.query(
			("INSERT INTO Tecajnica\n" +
			 "SELECT '%s'::date,\n" +
			 "papir_id, tecaj\n" +
			 "FROM Tecajnica\n" +
			 "WHERE datum='%s'\n" +  
			 "AND papir_id='%s'")  % (dan,st[0],st[1]))
	conn.query("commit")
    except:
	conn.query("rollback")
	raise sys.exc_type, sys.exc_value, sys.exc_traceback
    return None

def stof(s):
    try:
	return string.atof(s)
    except:
	return 0.0

def stoi(s):
    try:
	return string.atoi(s)
    except:
	return 0

def makeline(fmt, tuple):
    i = 0
    r = ''
    for f in fmt:
	if f=='f':
	    if tuple[i]==0:
		r = r + "<td>"
	    else:
		r = r + "<td align=right>" + Util.FormatFloat(tuple[i])
	elif f=='i':
	    if tuple[i]==0:
		r = r + "<td>"
	    else:
		r = r + "<td align=right>%d" % tuple[i]
	else:
	    r = r + "<td>" + tuple[i]
	i = i + 1
    return r

# Vrne teèajnico za dani dan:
# za vsako pogodbo vrne n-terico z naslednjimi polji:
# (povp. teèaj, najni¾ja cena, najvi¹ja cena, promet)
# Promet zaenkrat izpustimo
def Tecaji(dan='yesterday', refresh=0):
    global _TECAJIMAP

    # Izraèunaj datum
    datum = conn.query("SELECT '%s'::date AS datum" % dan)[0][0]
    
    if refresh:
	IzracunajTecaj(dan)
	try:
	    del _TECAJIMAP[datum]
	except:
	    pass

    if _TECAJIMAP.has_key(datum):
	tecaji = _TECAJIMAP[datum]
    else:
	db_tecaji = conn.query((
	    "SELECT papir_id,\n" +
	    "tecaj, mincena, maxcena, obseg\n" +
	    "FROM Tecajnica WHERE datum='%s'") % dan)
	if len(db_tecaji)==0:
	    IzracunajTecaj(dan)
	    db_tecaji = conn.query((
	    "SELECT papir_id,\n" +
	    "tecaj, mincena, maxcena, obseg\n" +
	    "FROM Tecajnica WHERE datum='%s'") % dan)

	tecaji = {}
	for k in db_tecaji:
	    if k[0]!='':
		tecaji[k[0]] = (stof(k[1]), stof(k[2]),
				stof(k[3]), stoi(k[4]))
	_TECAJIMAP[datum] = tecaji
    return tecaji

def HTMLTecaji(dan='yesterday'):
    tecaji = Tecaji(dan, 1)
    db_pap = conn.query("SELECT papir_id FROM papirji ORDER BY papir_id")
    
##     db_odprti = conn.query(
## 	"SELECT Papir_id,\n" + 
## 	"sum(kolicina)\n" +
## 	"FROM stanje\n" + 
## 	"WHERE kolicina>0 AND\n" +
## 	"datum<='%s'\n" +
## 	"GROUP BY Papir_id\n")	

##     odprti = {}
##     for k in db_odprti:
## 	try:
## 	    odprti[k[0]] = string.atoi(k[1])
## 	except:
## 	    pass

#    tb = '<colgroup align="char" char="." span=3>\n<colgroup align="right">'
    tb = "<thead><tr><th>Kandidat<th>Poravnalna cena<th>Najni¾ja cena<th>Najvi¹ja cena<th>Obseg\n<tbody>"
    # Naredi tabelo:
    # Mogoèe dodati ¹e spremembo teèaja v procentih?
    vsota_t=0.0
    vsota_o=0
    for k in db_pap:
	tb = tb + '<tr><th align="left">%s\n' % k
	if tecaji.has_key(k[0]):
	    tb = tb + makeline("fffi",tecaji[k[0]]) 
	    vsota_t = vsota_t + tecaji[k[0]][0]
	    vsota_o = vsota_o + tecaji[k[0]][3]
	else:
	    tb = tb + '<td><td><td><td>'
##	if odprti.has_key(k[0]):
##	    tb = tb + '<td align="right">%d\n' % odprti[k[0]]
##	else:
##	    tb = tb + '<td>\n'
	tb = tb + '\n'
    tb = tb + '<tr><th align="left">Vsota<td align="right">%s<td><td><td>%d\n' % (Util.FormatFloat(vsota_t), vsota_o)

    datum = conn.query("SELECT '%s'::date AS datum" % dan)[0][0]
    datum = Util.RewriteDate(datum)
    return  {'tecaj':tb, 'datum': datum}

def run(srcdir, destdir, templates, dan='yesterday'):
    dict = HTMLTecaji(dan)

    ime = Util.si2iso(dict['datum']) + '.html'

    # Preberi obrazec:
    templname = srcdir + '/' + templates['Tecaj']['ime'] + '.in'
    destname = destdir + templates['Tecaj']['dir'] + '/' + ime

    Util.MakeTemplate(templname, destname, dict)
    
    linktarg = destdir + templates['IndexTecaji']['dir'] + \
	       '/' + templates['Tecaj']['ime']
    try:
	os.unlink(linktarg)	       
    except:
	pass
    os.symlink(destname, linktarg)

    # Seznam vseh tecajnic:
    tdir = os.listdir(destdir + templates['Tecaj']['dir'])
    tdir.sort()
    tdir.reverse()
    tecajnice = ""
    topdir = templates['Tecaj']['dir']
    for t in tdir:
	tecajnice = tecajnice + \
		    '<li><a href="%s" target="tecaj">%s</a>' % (topdir + '/' + t,
						 Util.iso2si(t[:6]))

    templname = srcdir + '/' + templates['IndexTecaji']['ime'] + '.in'
    destname = destdir + templates['IndexTecaji']['dir'] + '/' + \
	       templates['IndexTecaji']['ime']
    
    dict['Tecajnice'] = tecajnice
    Util.MakeTemplate(templname, destname, dict)

    # Poskrbi ¹e za vse uporabnike:
#    import MakePregled
#
#    db_stranke = conn.query("select stranka_id from stranke")
#    for st in db_stranke:
#	MakePregled.defupdateuser(st[0])
