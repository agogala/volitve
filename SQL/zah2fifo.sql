insert into fifo
select cena, -5 AS kolicina,
	papir_id, datum, ura, ponudnik
from zahtevki
where
  oid=146656;
