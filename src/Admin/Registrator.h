/* -*- C++ -*-
 * $ProjectHeader: volitve 0.11 Thu, 11 Sep 1997 23:18:12 +0200 andrej $
 *
 * $Id: Registrator.h 1.1 Thu, 11 Sep 1997 16:28:32 +0000 andrej $
 *
 * Registrira stranke...
 *
 */

#if !defined(REGISTRATOR_H)
#define REGISTRATOR_H

#include <ace/Singleton.h>
#include <ace/Synch.h>
/*#include <ace/Process.h>
#include <ace/Event_Handler.h>
#include <ace/Reactor.h>
#include <ace/Pipe.h>*/

class RegRec 
{
  char *hash_;
  char *username_;
  char *passwd_;

public:
  RegRec(const char *hash, 
	 const char *username,
	 const char *passwd);
  ~RegRec();

  char *username();
  char *passwd();
  char *hash();
  
};

class Registrator
{
 public:
  Registrator ();
  int init();
  int Validate(const char *hash);
  int Register(RegRec &regrec);
  // Vrne oid uporabnika ali NULL. String se sprosti z delete.
  char *UserID(const char *user);

private:
  bool initialized_;

  // Tale poklièe python in vrne vrednost, ki jo vrne funkcija.
  int PythonRunInt(char *Command);
  char *PythonRunStr(char *Command);
};

typedef ACE_Singleton<Registrator, ACE_Null_Mutex>
        REGISTRATOR;

#endif // REGISTRATOR_H
