#!/net/rozle/export/share/sol/bin/python
# Izpi¹i stanja za vse:


def orderpair(a,b):
    if a[1]==b[1]:
	return string.lower(a[0])>string.lower(b[0])
    else:
	return a[1]<b[1]

import pg95
import os
import string

pg95.set_defbase("zupani")
conn = pg95.connect()

db_min = conn.query("select stranka_id, sum(kolicina) from stanje where kolicina<=0 and not papir_id='denar' group by stranka_id order by stranka_id")
db_plus = conn.query("select stranka_id, sum(kolicina) from stanje where kolicina>=0 and not papir_id='denar' group by stranka_id order by stranka_id")

sum = {}
for k in db_min:
    sum[k[0]] = - string.atoi(k[1])

for k in db_plus:
    if sum.has_key(k[0]):
	sum[k[0]] = sum[k[0]] + string.atoi(k[1])
    else:
	sum[k[0]] = string.atoi(k[1])

keys = sum.items()
keys.sort(orderpair)
for k in keys:
    print "%s: %-3d" % (k[0], k[1])

