/* -*- C++ -*- */
/*
 * $ProjectHeader: volitve 0.6 Fri, 05 Sep 1997 16:43:33 +0200 andrej $
 *
 * $Id: Request.h 1.2 Thu, 04 Sep 1997 02:15:14 +0000 andrej $
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
  Request(const char * rs);
  // Preberi iz trenutne vrstice:
  Request(PgDatabase &db, int tup_num);
  ~Request();

  // Preveri veljavnost (glede na trenutno bazo):
  bool IsValid(PgDatabase &db) const;
  
  // Preberi iz tabele:
  int Read(PgDatabase &db, int tup_num);
  // Shrani v tabelo, doloèi oid
  int Store(PgDatabase &db);

  // Atributi:
  const char *Ponudnik() const;
  const char *Papir_ID() const;
  // Vrne negativno vrednost, ce je nakup, pozitivno ce je prodaja.
  int Kolicina() const;
  double Cena() const;

  const char *ID() const;

  // Vrne razlog zakaj zadeva ni veljavna
  //  char *LastError() const;

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

  // Interne funkcije 
  int Read_i(PgDatabase &db, int tup_num);
  int Read_i(const char * rs);
};

#endif REQUEST_H
