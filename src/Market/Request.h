/* -*- C++ -*- */
/*
 * $ProjectHeader: volitve 0.18 Sun, 05 Oct 1997 22:53:12 +0200 andrej $
 *
 * $Id: Request.h 1.6 Fri, 03 Oct 1997 15:45:58 +0000 andrej $
 *
 * Zahtevek za blagovno borzo.
 *
 */

#if !defined (REQUEST_H)
#define REQUEST_H

#include <libpq++.h>
#include "Config.h"

class Request 
{
public:
  // Konstruktorji:
  Request();
  // Preberi string
  Request(char * rs);
  // Preberi iz trenutne vrstice:
  Request(PgDatabase &db, int tup_num);
  // Naredi kopijo
  //  Request(Request &req);
  // Sprosti spomin
  ~Request();

  // Preveri veljavnost (glede na trenutno bazo):
  bool IsValid(PgDatabase &db);
  
  // Preberi iz tabele:
  int Read(PgDatabase &db, int tup_num);
  // Shrani v tabelo, doloèi oid
  int Store(PgDatabase &db);

  // Atributi:
  const char *Ponudnik() const;
  const char *Papir_ID() const;
  // Vrne negativno vrednost, ce je prodaja, pozitivno ce je nakup.
  int Kolicina() const;
  double Cena() const;

  const char *ID() const;

  // Omejitev:
  int Omeji(PgDatabase &db);
  
  // Sprememba zahtevka. Samo v spominu.
  int Spremeni(int Kolicina);

  // Vrne razlog zakaj zadeva ni veljavna
  int LastError() const;

private:
  bool Valid_;

  char Ponudnik_[MAX_PONUDNIK + 1];
  char Papir_ID_[MAX_PAPIR_ID + 1];
  int Kolicina_;
  double Cena_;
  //  char Vrsta_;
  // Tole je potrebno ¹e razmisliti [...]
  //  date Datum_;
  //  time Ura_;

  // Enolièna oznaka.
  char ID_[MAX_ID + 1];

  int LastError_;

  // Interne funkcije 
  int Read_i(PgDatabase &db, int tup_num);
  int Read_i(char * rs);
};

#endif REQUEST_H
