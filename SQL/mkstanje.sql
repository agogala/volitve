-- Kartezièni produkt stanja in papirjev
insert into stanje (Stranka_ID, Papir_ID, Kolicina)
select D.Stranka_ID, D1.Papir_ID, 0 AS Kolicina
from Stranke D, Papirji D1;
-- Kopija strank:
insert into stanje (Stranka_ID, Papir_ID, Kolicina)
select D.Stranka_ID, 'denar' as Papir_ID, 0 AS Kolicina
from Stranke D;