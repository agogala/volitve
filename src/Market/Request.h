/* -*- C++ -*- */
/*
 * $ProjectHeader: volitve 0.14 Thu, 25 Sep 1997 21:32:05 +0200 andrej $
 *
 * $Id: Request.h 1.4 Wed, 24 Sep 1997 17:03:46 +0000 andrej $
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
  bool IsValid(PgDatabase &db) const;
  
  // Preberi iz tabele:
  int Read(PgDatabase &db, int tup_num);
  // Shrani v tabelo, dolo�i oid
  int Store(PgDatabase &db);

  // Atributi:
  const char *Ponudnik() const;
  const char *Papir_ID() const;
  // Vrne negativno vrednost, ce je nakup, pozitivno ce je prodaja.
  int Kolicina() const;
  double Cena() const;

  const char *ID() const;

  // Vrne razlog zakaj zadeva ni veljavna
  int LastError() const;

private:
  bool Valid_;

  char Ponudnik_[MAX_PONUDNIK + 1];
  char Papir_ID_[MAX_PAPIR_ID + 1];
  int Kolicina_;
  double Cena_;
  //  char Vrsta_;
  // Tole je potrebno �e razmisliti [...]
  //  date Datum_;
  //  time Ura_;

  // Enoli�na oznaka.
  char ID_[MAX_ID + 1];

  int LastError_;

  // Interne funkcije 
  int Read_i(PgDatabase &db, int tup_num);
  int Read_i(char * rs);
};

#endif REQUEST_H
