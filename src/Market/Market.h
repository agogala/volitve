// -*- C++ -*-
/*
 * $ProjectHeader: volitve 0.15 Fri, 26 Sep 1997 18:28:00 +0200 andrej $
 *
 * $Id: Market.h 1.4 Thu, 25 Sep 1997 19:32:05 +0000 andrej $
 *
 * Trg. Zna dodajati zahtevke.
 */

#if !defined(MARKET_H)
#define MARKET_H

#include <ace/Synch.h>
#include <ace/Singleton.h>
#include <libpq++.h>
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

#endif /* MARKET_H */
