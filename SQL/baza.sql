-- $ProjectHeader: volitve 0.12 Mon, 22 Sep 1997 15:21:03 +0200 andrej $
--
-- $Id: baza.sql 1.3.2.4 Mon, 22 Sep 1997 13:21:03 +0000 andrej $
--
-- Skreira bazo.

-- Papirji:
-- Zaenkrat samo imena. Moram dodati ¹e min, max vrednost, rok trajanja
CREATE TABLE Papirji
	(Papir_ID	char(8));

-- Zagotavlja enoliènost ¹ifer:
CREATE UNIQUE INDEX Papir_ID
	ON Papirji (Papir_ID);

-- Stranke: (naziv je verjetno odveè
CREATE TABLE Stranke
	(Stranka_ID	char(10),
	Naziv		text,
	EMail		text);

-- Enoliènost nazivov:
CREATE UNIQUE INDEX Stranka_ID
	ON Stranke (Stranka_ID);

-- Podatki o prispelih registracijah:
CREATE TABLE Registracije
	(EMail		text,
	hash		char(32),
	accessed	bool);

-- Stanje na raèunu:
-- Papir_ID je lahko prazen, to pomeni izkupicek.
CREATE TABLE Stanje
	(Stranka_ID	char(10),
	Papir_ID	char(8),
	Kolicina	int,
	Cena		float));	

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
