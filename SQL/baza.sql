CREATE TABLE Fifo 
	(Cena 		float,	
	Kolicina	int,
	Papir_ID	char(4),
	Datum		date,
	Ura		time,
	Ponudnik	char(10));

CREATE TABLE Zahtevki
	(Cena 		float,	
	Kolicina	int,
	Papir_ID	char(4),
	Datum		date,
	Ura		time,
	Ponudnik	char(10));

CREATE TABLE Papirji
	(Papir_ID	char(4));

CREATE UNIQUE INDEX Papir_ID
	ON Papirji (Papir_ID);

CREATE TABLE Stranke
	(Stranka_ID	char(10),
	Naziv		text,
	EMail		text);

CREATE UNIQUE INDEX Stranka_ID
	ON Stranke (Stranka_ID);

CREATE TABLE Posli
	(Datum		date,
	Ura		time,
	Papir_ID	char(4),
	Cena		float,
	Kolicina	int,
	Kupec		char(10),
	Prodajalec	char(10));

