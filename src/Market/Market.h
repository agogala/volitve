// -*- C++ -*-
/*
 * $ProjectHeader: volitve 0.28 Sat, 27 Dec 1997 16:06:49 +0100 andrej $
 *
 * $Id: Market.h 1.6 Sun, 19 Oct 1997 17:07:54 +0000 andrej $
 *
 * Trg. Zna dodajati zahtevke.
 */

#if !defined(MARKET_H)
#define MARKET_H

#include <ace/Synch.h>
#include <ace/Singleton.h>
#include <libpq++.h>
#include "Stanje.h"
#include "Request.h"
#include "StrSet.h"

class Market
{
 public:
  Market();
  ~Market();
  
  /* Odpri povezavo z bazo */
  int open(void);

  /* Dodaj zahtevek */
  int Add(Request &req, strset *userset = NULL);

  // Preklièi zahtevek
  int Cancel(Request &req, strset *userset = NULL);

  char *Result();

  int LastRC();

 private:
  int LastRC_;
  char Result_[256];
  PgDatabase *db;

  // Interna funkcija:
  bool FixFIFO(char *KolOp, const char *Papir_ID, 
	       char *Urejenost, strset *userset = NULL);

};

// Market singleton
typedef ACE_Singleton<Market, ACE_Null_Mutex>
	MARKET;

typedef ACE_Singleton<ShrambaStanj, ACE_Null_Mutex>
        STANJA;

#endif /* MARKET_H */
