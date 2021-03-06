# $ProjectHeader: volitve 0.28 Sat, 27 Dec 1997 16:06:49 +0100 andrej $
#
# $Id: MakePregled.py 1.13 Fri, 21 Nov 1997 17:06:57 +0000 andrej $
# Naredi dokument pregled.html: zares opravi delo.

import pg95
import admin_cfg
import Util
import DBConn
import Registrator
import string
import time
import MakeTecaj

# Imamo kar stalno povezavo z bazo:
conn = DBConn.db_conn

def CollectCols(list, kol='<td align="right">', links=''):
    val = ""
    if links!='':
	cnt = 1
	for l in list:
	    val = val + kol + '<a href=/trg/pogodbe/%s>' % links[cnt] + l + "</a>\n"
	    cnt = cnt + 1
    else:
	for l in list:
	    val = val + kol + l + "\n"
    return val

def run(srcdir, destdir, templates):
    db_ponudbe = conn.query("SELECT min(cena), papir_id FROM fifo WHERE kolicina<0 GROUP BY papir_id ORDER BY papir_id")
    ponudbe = {}
    for p in db_ponudbe:
	ponudbe[p[1]] = p[0]

    db_povpr = conn.query("SELECT max(cena), papir_id FROM fifo WHERE kolicina>0 GROUP BY papir_id ORDER BY papir_id")
    povpr = {}
    for p in db_povpr:
	povpr[p[1]] = p[0]

    db_pap = conn.query("SELECT papir_id FROM papirji ORDER BY papir_id")

    tecaj = MakeTecaj.Tecaji('today', 1)

    # Dolo?imo ?tevilo poslov v zadnji uri, max in min ceno v zadnji uri.    
    db_zadnjaura = conn.query("SELECT papir_id, max(cena), min(cena), count(kolicina) FROM posli WHERE abstime(datetime(datum,ura)) > ('now'::abstime - '1hour'::reltime) GROUP BY papir_id ORDER by papir_id")

    zadnjaura = {}
    for p in db_zadnjaura:
	zadnjaura[p[0]] = (p[1],p[2],p[3])

    html_head="<thead><tr><th>"
    html_pon='\n<tbody><tr><th align=left><font color="red">Ponudbe</font>'
    html_povp='\n<tr><th align=left><font color="blue">Povpr.</font>'
    html_cena="\n<tr><th align=left>Por. cena"
    html_posl="\n<tr><th align=left>?tevilo"
    html_max="\n<tr><th align=left>Max. cena"
    html_min="\n<tr><th align=left>Min. cena"
    vsota=0
    for p in db_pap:
	html_head=html_head + '<th>' + p[0] + '\n'
	html_pon = html_pon + '<td align="right">' 
	html_povp = html_povp + '<td align="right">'
	html_cena = html_cena + '<td align="right">'
	html_posl = html_posl + '<td align="right">'
	html_max = html_max + '<td align="right">'
	html_min = html_min + '<td align="right">'
	if ponudbe.has_key(p[0]):	    
	   html_pon=html_pon + Util.FormatFloat(string.atof(ponudbe[p[0]]))
	if povpr.has_key(p[0]):
	    html_povp=html_povp + Util.FormatFloat(string.atof(povpr[p[0]]))
	if tecaj.has_key(p[0]):
	    html_cena=html_cena + Util.FormatFloat(tecaj[p[0]][0])
	    vsota = vsota + tecaj[p[0]][0]
	if zadnjaura.has_key(p[0]):
	    html_posl = html_posl + zadnjaura[p[0]][2]
	    html_max = html_max + Util.FormatFloat(string.atof(zadnjaura[p[0]][0]))
	    html_min = html_min + Util.FormatFloat(string.atof(zadnjaura[p[0]][1]))

##     FIFO = "<thead><tr><th><th>Ponudba<th>Povpra?evanje</tr>\n<tbody><tr>\n"
##     for p in db_pap:
## 	if ponudbe.has_key(p[0]):
## 	    pon = Util.FormatFloat(string.atof(ponudbe[p[0]]))
## 	else:
## 	    pon = ""
## 	if povpr.has_key(p[0]):
## 	    pov = Util.FormatFloat(string.atof(povpr[p[0]]))
## 	else:
## 	    pov = ""
## 	FIFO = FIFO + '<tr>\n<th align="left">%s\n<td align="right">%s\n<td align="right">%s\n</tr>\n' % (p[0], pon, pov)

    html_head = html_head + '<th>Vsota\n'
    html_cena = html_cena + '<td align="right">' + Util.FormatFloat(vsota)
    FIFO = html_head + html_pon + html_povp + html_cena + html_max + html_min + html_posl

    t = time.localtime(time.time())
    datum = time.strftime('%d.%m.%y',t)
    ura = time.strftime('%H:%M',t)


    # Preberi obrazec:
    templname = srcdir + '/' + templates['Pregled']['ime'] + '.in'
    destname = destdir + templates['Pregled']['dir'] + '/' + \
		templates['Pregled']['ime'] 
    Util.MakeTemplate(templname, destname, {'FIFO':FIFO,
					    'datum':datum,
					    'ura':ura})

def MakeUserRow(f):
    val = "<tr>\n"
    # OID:
    val = val + '<td><input type="checkbox" name="%s">\n' % f[0]
    #Papir:
    val = val + '<th align="left">\n' + f[1]
    # Cena
    val = val + '<td align="right">\n' + Util.FormatFloat(string.atof(f[2]))
    # Koli?ina
    val = val + '<td align="right">\n' + f[3]
    # Datum
    val = val + '<td>\n' + Util.RewriteDate(f[4])
    # Ura
    val = val + '<td>\n' + f[5]
    val = val + "</tr>\n"
    return val

def updateuser(srcdir, destdir, templates, user):

    user = user[:10]

    usr_ponudbe = conn.query("SELECT oid, papir_id, cena, -kolicina, datum, ura FROM fifo WHERE ponudnik='%s' AND kolicina<0 ORDER BY papir_id, cena, datum, ura" % user)

    if len(usr_ponudbe)>0:
	FIFO = '<font color="red">Ponudbe:</font>\n<table cellspacing="1" bgcolor="lightblue" border="1">\n'
	FIFO = FIFO + "<thead><tr><th>Prekli?i<th>Papir<th>Cena<th>Koli?ina<th>Datum<th>Ura</tr><tbody>\n"
	for f in usr_ponudbe:
	    FIFO = FIFO + MakeUserRow(f)
	FIFO = FIFO + '</table><br>'
    else:
	FIFO = ""

    usr_povpr = conn.query("SELECT oid, papir_id, cena, kolicina, datum, ura FROM fifo WHERE ponudnik='%s' AND kolicina>0 ORDER BY papir_id, cena, datum, ura" % user)
    
    if len(usr_povpr)>0:
	FIFO = FIFO + '<font color="blue">Povpra?evanja:</font>\n<table cellspacing="1" bgcolor="lightblue" border="1">\n'
	FIFO = FIFO + "<tr><th>Prekli?i<th>Papir<th>Cena<th>Koli?ina<th>Datum<th>Ura</tr>\n"
	for f in usr_povpr:
	    FIFO = FIFO + MakeUserRow(f)
	FIFO = FIFO + "</table>\n<br>"

    if FIFO!="":
	FIFO = FIFO + '<input type="submit" name="Zbrisi" value="  Preklic  ">'

    usr_stanje = conn.query("SELECT * FROM stanje WHERE stranka_id='%s' ORDER BY papir_id" % user)
    Papirji = ['']
    Kolicine = []
    Cena = []
    tecaj = MakeTecaj.Tecaji('today')
    denar = 0
    vsota = 0
    for f in usr_stanje:
	if string.strip(f[1])<>"denar":
	    Papirji.append(f[1])
	    Kolicine.append(f[2])
	    vsota = vsota + abs(string.atoi(f[2]))
	    if tecaj.has_key(f[1]):
		Cena.append(Util.FormatFloat(tecaj[f[1]][0]))
		denar = denar + string.atof(f[2]) * tecaj[f[1]][0]
	    else:
		Cena.append('')
	else:
	    denar = denar + string.atof(f[2])
	    posli = Util.FormatFloat(string.atof(f[2]))

    # Vsota koli?in in stanje:
    Papirji2 = ['Vsota', 'SIT']
    Kolicine2 = ['%d' % vsota, posli]
    Cena2 = ['', Util.FormatFloat(denar)] 

    Stanje = "<thead><tr>" + CollectCols(Papirji, "<th>") 
    Stanje = Stanje + CollectCols(Papirji2, "<th>") + "</tr>\n"
    Stanje = Stanje + "<tbody><tr><th>Koli?ina" + CollectCols(Kolicine, '<td align="right">', Papirji) 
    Stanje = Stanje + CollectCols(Kolicine2) + "</tr>\n"
    Stanje = Stanje + "<tr><th>Por. cena" + CollectCols(Cena) 
    Stanje = Stanje + CollectCols(Cena2) + "</tr>\n"

    (result, userid) = Registrator.UserID(user)
    if result!=0: # Error selecting user!
	return

    t = time.localtime(time.time())
    datum = time.strftime('%d.%m.%y',t)
    ura = time.strftime('%H:%M',t)
    
    # Preberi obrazec:
    templname = srcdir + '/' + \
		admin_cfg.templates['Stanje']['ime'] + '.in'
    destname = destdir + '/' + userid + '/index.html'
    Util.MakeTemplate(templname, destname, {'username': user,
					    'FIFO': FIFO,
					    'Stanje': Stanje,
					    'Datum': datum,
					    'Ura': ura})
    
	
def defupdateuser(user):
    userroot = admin_cfg.htmldir + \
	       admin_cfg.templates['Stanje']['dir'] 
    updateuser(admin_cfg.tempdir, userroot,
	       admin_cfg.templates, user)
