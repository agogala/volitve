// -*- C++ -*-
/*
 * $ProjectHeader: volitve 0.17 Fri, 03 Oct 1997 17:45:58 +0200 andrej $
 *
 * $Id: Market.h 1.5 Fri, 03 Oct 1997 15:45:58 +0000 andrej $
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

  char *Result();

  int LastRC();

 private:
  int LastRC_;
  char Result_[256];
  PgDatabase *db;
};

// Market singleton
typedef ACE_Singleton<Market, ACE_Null_Mutex>
	MARKET;

typedef ACE_Singleton<ShrambaStanj, ACE_Null_Mutex>
        STANJA;

#endif /* MARKET_H */
