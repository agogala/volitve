// -*- C++ -*-
/*
 * $ProjectHeader: volitve 0.2 Thu, 28 Aug 1997 23:38:14 +0200 andrej $
 *
 * $Id: Request.h 1.2 Thu, 28 Aug 1997 21:38:14 +0000 andrej $
 *
 * Zahtevek. Nikakor ne bi delal v veè nitnem okolju. Treba premisliti.
 */

#if !defined(REQUEST_H)
#define REQUEST_H

#include <ace/Synch.h>
#include <ace/Singleton.h>
#include <libpq++.h>

class Request
{
 public:
  Request();
  ~Request();
  
  /* Odpri povezavo z bazo */
  int open(void);

  /* Dodaj zahtevek */
  int Add(char *Line);

  char *Result();

  int LastRC();

 private:
  int LastRC_;
  char Result_[256];
  PgDatabase *db;
};

// Request singleton
typedef ACE_Singleton<Request, ACE_Null_Mutex>
	REQUEST;

#endif /* REQUEST_H */
