/* -*- C++ -*-
 * $ProjectHeader: volitve 0.23 Tue, 28 Oct 1997 21:15:29 +0100 andrej $
 *
 * $Id: Stanje.h 1.2 Sun, 19 Oct 1997 17:07:54 +0000 andrej $
 *
 * Definira stanje in hrambo stanj.
 */

#if !defined(STANJE_H)
#define STANJE_H

#include <libpq++.h>
#include <map.h>
#include <String.h>
#include "Utils.h"

#define denar_ID "denar"

// Interval dovoljenih vrednosti
typedef pair<int, int> Interval;

typedef map<const char*, double, ltstr> StanjeContent;

// Stanje neke stranke. Vedno se prebere iz baze.
class Stanje : public StanjeContent
{
  char *User_ID_;

public:

  Stanje() { User_ID_ = NULL; };
  Stanje(const Stanje& s);
  ~Stanje() { delete User_ID_; };

  // Prireditev:
  Stanje& operator=(const Stanje&);

  // Preberi iz baze
  int Read(PgDatabase &db, const char *User_ID);

  // Omejitve kolièine
  Interval Dovoljeno(PgDatabase &db, const char* Papir_ID);
  int Omeji(PgDatabase &db, const char* Papir_ID, int Kolicina);

  // Spreminjanje stanja
  int Spremeni(PgDatabase &db, const char* Papir_ID, 
	       int Kolicina, double Cena);

  char *User_ID();
};

class ShrambaStanj: public map<const char*, Stanje*, ltstr>
{
public:
  // Vrne iterator na stanje za dano stranko 
  iterator Get(PgDatabase &db, const char* User_ID);
};

#endif
