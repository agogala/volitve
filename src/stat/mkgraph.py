#!/net/rozle/export/share/sol/bin/python

import pg95
import os
import string
import copy

pg95.set_defbase("volitve");
conn = pg95.connect();
grbatch="/net/rozle/export/share/sol/bin/grbatch"
tmpdir="tmp/"
outdir="/opt/borza/HTML/stat/"
#outdir="tmp/"
converter="/net/rozle/export/share/sol/bin/convert"
pardir="/opt/borza/wwwstat/"

def FixDate(date):
    return  date + (4712 + 1997)*365 + 1500 + 480 - 9
#    return date[3:5] + '.' + date[0:2]


def MkGraph(data, name, flags=""):
    f = open(tmpdir + name + ".dat", "w")
    f.write(data)
    f.close()
    os.system("%(grbatch)s -p %(pardir)s%(name)s.par -device 1 -eps -noask -printfile %(tmpdir)s%(name)s.eps %(flags)s %(tmpdir)s%(name)s.dat" % {
	'grbatch': grbatch,
	'name': name,
	'tmpdir': tmpdir,
	'flags': flags,
	'pardir': pardir})
    os.system("%(converter)s -rotate 90 %(tmpdir)s%(name)s.eps %(outdir)s%(name)s.gif" % {
	'converter': converter,
	'tmpdir': tmpdir,
	'name': name,
	'outdir': outdir})
    os.system("%(converter)s -geometry 100x100 -colors 16 %(outdir)s%(name)s.gif %(outdir)s%(name)s-chip.gif" % { 
	'converter': converter,
	'outdir': outdir,
	'name': name})

# vsota vseh poravnalnih cen:
# tu ugotovimo tudi ¹tevilo dnevov:
db_vsota = conn.query("select datum, sum(tecaj) from tecajnica where datum<'today' group by datum order by datum")

empty = []
for d in db_vsota:
    empty.append(0)

# Papirji:
db_papirji = conn.query("select papir_id from papirji order by papir_id");
data_cene = {}
for p in db_papirji:
    data_cene[p[0]] = copy.copy(empty)


str_vsota = ""
data_vsota = {}
datumi = []

for i in db_vsota:
    #    s = %s % FixDate(i[0])
    datum = "%d" % FixDate(db_vsota.index(i))
    datumi.append(i[0])
    data_vsota[i[0]]= string.atof(i[1])
    str_vsota = str_vsota + datum
    str_vsota = str_vsota + " " + i[1]
    str_vsota = str_vsota + "\n"    

db_tecaji = conn.query("select datum, papir_id, tecaj from tecajnica where datum<'today' order by datum, papir_id")

for t in db_tecaji:
    data_cene[t[1]][datumi.index(t[0])] = string.atof(t[2])

str_porazd = ""
str_cene = ""

for i in db_vsota:
    datum = "%d" % FixDate(db_vsota.index(i))
    str_cene = str_cene + datum
    str_porazd = str_porazd + datum
    for p in db_papirji:
	tecaj = data_cene[p[0]][datumi.index(i[0])]
	vsota = data_vsota[i[0]]
	str_cene = str_cene + " %f" % (tecaj)
	str_porazd = str_porazd + " %f" % (tecaj/vsota * 100)
    str_cene = str_cene + "\n"
    str_porazd = str_porazd + "\n"

MkGraph(str_vsota, "vsota")
MkGraph(str_cene, "cene", "-nxy")
MkGraph(str_porazd, "przd", "-nxy")
