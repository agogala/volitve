/* -*- C++ -*- */
/*
 * $ProjectHeader: volitve D.2 Wed, 03 Sep 1997 07:39:11 +0200 andrej $
 *
 * $Id: Request.h 1.1 Wed, 03 Sep 1997 05:39:11 +0000 andrej $
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
  Request(PgDatabase &db);
  ~Request();

  // Preveri veljavnost (glede na trenutno bazo):
  bool IsValid(PgDatabase &db) const;
  
  // Preberi iz tabele:
  int Read(PgDatabase &db);
  // Shrani v tabelo:
  int Store(PgDatabase &db) const;

  // Atributi:
  const char *Ponudnik() const;
  const char *Papir_ID() const;
  // Vrne negativno vrednost, ce je nakup, pozitivno ce je prodaja.
  unsigned int Kolicina() const;
  double Cena() const;

  // Vrne razlog zakaj zadeva ni veljavna
  //  char *LastError() const;

private:
  bool Valid_;

  char Ponudnik_[MAX_PONUDNIK];
  char Papir_ID_[MAX_PAPIR_ID];
  unsigned int Kolicina_;
  double Cena_;
  char Vrsta_;
  //  date Datum_;
  //  time Ura_;

  //  String LastError_;

  // Interne funkcije 
  int Read_i(PgDatabase &db);
  int Read_i(char * rs);
};

#endif REQUEST_H
