/*
 * $ProjectHeader: volitve 0.13 Wed, 24 Sep 1997 19:03:46 +0200 andrej $
 *
 * $Id: observerd.cpp 1.5 Tue, 09 Sep 1997 22:58:50 +0000 andrej $
 *
 * Observer deamon. 
 */

#include <ace/INET_Addr.h>

#include "observerd.h"
#include "Notification_Handler.h"
#include "Formater.h"
#include "State.h"

#include "Config.h"

int 
main (int argc, char *argv[])
{  

  State state;

  Notification_Handler notification(ACE_INET_Addr(NOTIFIER_DEFAULT_PORT), 
				    &state);
  Formater formater(&state);

  if (REACTOR::instance ()->register_handler
      (SIGINT, QUIT_HANDLER::instance()) == -1)
   ACE_ERROR_RETURN ((LM_ERROR,
		      "registering service with ACE_Reactor\n"), -1);
  else if (REACTOR::instance ()->register_handler
	   (&notification, ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
		       "can'(%P|%t) can't register with reactor\n"), -1);
  else if (REACTOR::instance ()->schedule_timer
	   (&formater,
	    NULL,
	    ACE_Time_Value(1),
	    ACE_Time_Value(30)) == -1)
    ACE_ERROR_RETURN((LM_ERROR,
		      "can'(%P|%t) can't register with reactor\n"), -1);
  ACE_DEBUG ((LM_DEBUG,
	      "(%P|%t) starting up observer daemon\n"));
  
  // Perform logging service until QUIT_HANDLER receives SIGINT.
  while (QUIT_HANDLER::instance () ->is_set () == 0)
    REACTOR::instance ()->handle_events ();
  
  ACE_DEBUG ((LM_DEBUG,
	      "(%P|%t) shutting down observer daemon\n"));
  
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

