/*
 * $ProjectHeader: volitve 0.27 Fri, 21 Nov 1997 18:06:57 +0100 andrej $
 *
 * $Id: observerd.cpp 1.10 Tue, 28 Oct 1997 20:15:29 +0000 andrej $
 *
 * Observer deamon. 
 */

#include <ace/INET_Addr.h>

#include "observerd.h"
#include "Notification_Handler.h"
#include "Formater.h"
#include "State.h"
#include "StrSet.h"

#include "Config.h"

Formater *formater;

int 
main (int argc, char *argv[])
{  

  ACE_Sig_Set signals;

  signals.sig_add(SIGINT);
  signals.sig_add(SIGTERM);
  signals.sig_add(SIGHUP);

  State state;
  strset userset;

  Notification_Handler notification(ACE_INET_Addr(NOTIFIER_DEFAULT_PORT), 
				    &state, &userset);
  formater = new Formater(&state, &userset);

  Formater_Acceptor formater_acceptor;

  if (REACTOR::instance ()->register_handler
      (signals, QUIT_HANDLER::instance()) == -1)
   ACE_ERROR_RETURN ((LM_ERROR,
		      "registering service with ACE_Reactor\n"), -1);
  else if (REACTOR::instance ()->register_handler
	   (&notification, ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
		       "can'(%P|%t) can't register with reactor\n"), -1);
  else if (REACTOR::instance ()->schedule_timer
	   (formater,
	    NULL,
	    ACE_Time_Value(2),
	    ACE_Time_Value(2)) == -1)
    ACE_ERROR_RETURN((LM_ERROR,
		      "can'(%P|%t) can't register with reactor\n"), -1);
  else  if (formater_acceptor.open
      (ACE_UNIX_Addr ( FORMATER_DEFAULT_PATH ),
       REACTOR::instance ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open"), -1);
  else if (REACTOR::instance ()->register_handler
	   (&formater_acceptor, ACE_Event_Handler::ACCEPT_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
		       "can'(%P|%t) can't register with reactor\n"), -1);

  ACE_DEBUG ((LM_DEBUG,
	      "(%P|%t) starting up observer daemon\n"));
  
  // Perform logging service until QUIT_HANDLER receives SIGINT.
  while (QUIT_HANDLER::instance () ->is_set () == 0)
    REACTOR::instance ()->handle_events ();
  
  ACE_DEBUG ((LM_DEBUG,
	      "(%P|%t) shutting down observer daemon\n"));

  ACE_OS::unlink( FORMATER_DEFAULT_PATH );
  
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton<ACE_Reactor, ACE_Null_Mutex>;
template class ACE_Singleton<ACE_Test_and_Set <ACE_Null_Mutex, sig_atomic_t>, ACE_Null_Mutex>;
template class ACE_Test_and_Set<ACE_Null_Mutex, sig_atomic_t>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton<ACE_Reactor, ACE_Null_Mutex>
#pragma instantiate ACE_Singleton<ACE_Test_and_Set <ACE_Null_Mutex, sig_atomic_t>, ACE_Null_Mutex>
#pragma instantiate ACE_Test_and_Set<ACE_Null_Mutex, sig_atomic_t>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

