#!/net/rozle/export/share/sol/bin/python
# Popravi stanje na raèunu!

import pg95
pg95.set_defbase("volitve")
conn = pg95.connect()

conn.query("begin")
try:
    kupci=conn.query("select float8(sum(kolicina)) as kolicina, kupec, papir_id \
    from posli \
    group by kupec, papir_id")

    for k in kupci:
	upd = conn.query("update stanje set kolicina=kolicina + %s \
    where stranka_id='%s' and papir_id='%s'" % k)

    prodajalci=conn.query("select -float8(sum(kolicina)) as kolicina, \
    prodajalec, papir_id \
    from posli \
    group by prodajalec, papir_id")

    for p in prodajalci:
	upd = conn.query("update stanje set kolicina=kolicina + %s \
    where stranka_id='%s' and papir_id='%s'" % p)

    #denar:
    kupcidenar=conn.query("select -sum(float8(kolicina)*cena) as kolicina, \
    kupec, 'denar' as papir_id \
    from posli \
    group by kupec")

    for k in kupcidenar:
	upd = conn.query("update stanje set kolicina=kolicina + %s \
    where stranka_id='%s' and papir_id='%s'" % k)

    prodajalcidenar=conn.query("select sum(float8(kolicina)*cena) as kolicina, \
    prodajalec, 'denar' as papir_id \
    from posli \
    group by prodajalec")

    for p in prodajalcidenar:
	upd = conn.query("update stanje set kolicina=kolicina + %s \
    where stranka_id='%s' and papir_id='%s'" % p)
    conn.query("commit")
except:
    conn.query("rollback")
