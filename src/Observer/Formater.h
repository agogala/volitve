/* -*- C++ -*-
 * $ProjectHeader: volitve 0.17 Fri, 03 Oct 1997 17:45:58 +0200 andrej $
 *
 * $Id: Formater.h 1.2 Fri, 26 Sep 1997 16:28:00 +0000 andrej $
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
  /*  virtual int handle_signal (int signum,
                             siginfo_t * = 0,
                             ucontext_t * = 0
                             );*/
private:
  State *state_;
  strset *userset_;
  bool initialized_;
  //  int mkpregled();
};

/*typedef ACE_Singleton<Formater, ACE_Null_Mutex>
        FORMATER;
        */
#endif // Formater_H
