// -*- C++ -*-
/*
 * $ProjectHeader: volitve 0.13 Wed, 24 Sep 1997 19:03:46 +0200 andrej $
 *
 * $Id: Market.h 1.3 Wed, 03 Sep 1997 05:39:11 +0000 andrej $
 *
 * Trg. Zna dodajati zahtevke.
 */

#if !defined(MARKET_H)
#define MARKET_H

#include <ace/Synch.h>
#include <ace/Singleton.h>
#include <libpq++.h>
#include "Request.h"

class Market
{
 public:
  Market();
  ~Market();
  
  /* Odpri povezavo z bazo */
  int open(void);

  /* Dodaj zahtevek */
  int Add(Request &req);

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
