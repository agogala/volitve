/* -*- C++ -*-
 * $ProjectHeader: volitve 0.27 Fri, 21 Nov 1997 18:06:57 +0100 andrej $
 *
 * $Id: Registrator.h 1.2 Mon, 22 Sep 1997 13:21:03 +0000 andrej $
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
/*
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
*/
class Registrator
{
 public:
  Registrator ();
  int init();
  int Validate(const char *hash);
  int Register(char *hash, char *username, char *passwd);
  int UserID(const char *user, char *ID);

private:
  bool initialized_;

  // Tale poklièe python in vrne vrednost, ki jo vrne funkcija.
  int PythonRunInt(char *Command);
  int PythonRunStr(char *Command, char *Result);
};

typedef ACE_Singleton<Registrator, ACE_Null_Mutex>
        REGISTRATOR;

#endif // REGISTRATOR_H
