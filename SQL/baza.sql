-- $ProjectHeader: volitve 0.10 Thu, 11 Sep 1997 18:28:32 +0200 andrej $
--
-- $Id: baza.sql 1.3.2.2 Thu, 11 Sep 1997 16:28:32 +0000 andrej $
--
-- Skreira bazo.

-- Papirji:
-- Zaenkrat samo imena. Moram dodati �e min, max vrednost, rok trajanja
CREATE TABLE Papirji
	(Papir_ID	char(8));

-- Zagotavlja enoli�nost �ifer:
CREATE UNIQUE INDEX Papir_ID
	ON Papirji (Papir_ID);

-- Stranke: (naziv je verjetno odve�
CREATE TABLE Stranke
	(Stranka_ID	char(10),
	Naziv		text,
	EMail		text);

-- Enoli�nost nazivov:
CREATE UNIQUE INDEX Stranka_ID
	ON Stranke (Stranka_ID);

-- Podatki o prispelih registracijah:
CREATE TABLE Registracije
	(EMail		text,
	hash		char(8),
	accessed	bool);

-- Stanje na ra�unu:
-- Papir_ID je lahko prazen, to pomeni izkupicek.
CREATE TABLE Stanje
	(Stranka_ID	char(10),
	Papir_ID	char(8),
	Kolicina	int,
	Cena		float))	

-- Sklenjeni posli:
CREATE TABLE Posli
	(Datum		date,
	Ura		time,
	Papir_ID	char(8),
	Cena		float,
	Kolicina	int,
	Kupec		char(10),
	Prodajalec	char(10));

-- Odprti posli:
CREATE TABLE Fifo 
	(Cena 		float,	
	Kolicina	int,
	Papir_ID	char(8),
	Datum		date,
	Ura		time,
	Ponudnik	char(10));

-- Zahtevki:
CREATE TABLE Zahtevki
	(Cena 		float,	
	Kolicina	int,
	Papir_ID	char(8),
	Datum		date,
	Ura		time,
	Ponudnik	char(10));
