/*
 * $ProjectId$
 *
 * $Id: Request.h 1.1 Thu, 28 Aug 1997 17:15:39 +0000 andrej $
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
  
  /* Dodaj zahtevek */
  int Add(char *Line);

  char *Result();

  int LastRC();

 private:
  int LastRC_;
  char Result_[256];
};

// Request singleton
typedef ACE_Singleton<Request, ACE_Null_Mutex>
	REQUEST;

#endif /* REQUEST_H */
