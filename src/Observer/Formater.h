/* -*- C++ -*-
 * $ProjectHeader: volitve 0.7 Mon, 08 Sep 1997 17:37:41 +0200 andrej $
 *
 * $Id: Formater.h 1.1 Mon, 08 Sep 1997 15:37:41 +0000 andrej $
 *
 * Oblikuje HTML datoteke. Se spro�a ob regularnih �asovnih razmikih.
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

class Formater: public ACE_Event_Handler
{
 public:
  Formater (State *state);
  int init();
  virtual int handle_timeout(const ACE_Time_Value &,
                                 const void *arg);
  /*  virtual int handle_signal (int signum,
                             siginfo_t * = 0,
                             ucontext_t * = 0
                             );*/
private:
  State *state_;
  bool initialized_;
  //  int mkpregled();
};

/*typedef ACE_Singleton<Formater, ACE_Null_Mutex>
        FORMATER;
        */
#endif // Formater_H