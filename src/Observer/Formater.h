/* -*- C++ -*-
 * $ProjectHeader: volitve 0.24 Mon, 03 Nov 1997 14:25:50 +0100 andrej $
 *
 * $Id: Formater.h 1.3 Tue, 28 Oct 1997 20:15:29 +0000 andrej $
 *
 * Oblikuje HTML datoteke. Se spro¾a ob regularnih èasovnih razmikih.
 * Datoteke formatira python script, s katerim tale objekt komunicira
 * prek pipeov.
 */

#if !defined(FORMATER_H)
#define FORMATER_H

/*#include <ace/Singleton.h>
#include <ace/Synch.h>
#include <ace/Process.h>*/
#include <ace/Acceptor.h>
#include <ace/LSOCK_Acceptor.h>
#include <ace/Synch.h>
#include <ace/Event_Handler.h>
/*#include <ace/Reactor.h>
#include <ace/Pipe.h>*/

#include "State.h"
#include "StrSet.h"

class Formater: public ACE_Event_Handler
{
 public:
  Formater (State *state, strset *userset);
  int init();

  virtual int handle_timeout(const ACE_Time_Value &,
                                 const void *arg);

  int mkPregled();
  int mkUser(const char *User);

private:
  State *state_;
  strset *userset_;
  bool initialized_;

};

class Formater_Handler: public ACE_Svc_Handler<ACE_LSOCK_STREAM, ACE_NULL_SYNCH>
{
public:
  // = Hooks for opening and closing handlers.
  virtual int open (void *);
  virtual int handle_input (ACE_HANDLE);
};

// Specialize a Formater Acceptor.
typedef ACE_Acceptor <Formater_Handler, ACE_LSOCK_ACCEPTOR>
	Formater_Acceptor;


/*typedef ACE_Singleton<Formater, ACE_Null_Mutex>
        FORMATER;
        */
#endif // Formater_H
