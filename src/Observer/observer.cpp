/* 
 * $ProjectHeader: volitve 0.6 Fri, 05 Sep 1997 16:43:33 +0200 andrej $
 *
 * $Id: observer.cpp 1.3 Fri, 05 Sep 1997 14:43:33 +0000 andrej $
 *
 * Èaka na obvestila o spremembah na trgu in potem zgenerira potrebne 
 * dokumente in obve¹èa kliente, ki so se prikljuèili nanj.
 */

#include <ace/Reactor.h>
#include <ace/Synch.h>
#include <ace/Singleton.h>
/*#include <ace/Log_Msg.h>*/
#include <ace/SOCK_Dgram.h>
#include <ace/INET_Addr.h>
#include <ace/Process.h>

#if defined (ACE_HAS_MINIMUM_IOSTREAMH_INCLUSION)
# include <iostream.h>
#endif /* ACE_HAS_MINIMUM_IOSTREAMH_INCLUSION */


#include "Config.h"

class Observer_Handler : public ACE_Event_Handler, public ACE_SOCK_Dgram
{
  // = TITLE
  //     Receive broadcast messages from market.
  //
  // = DESCRIPTION
  //     This class waits for broadcast messages from market and sets 
  //     appropriate flags.
public:
  Observer_Handler (const ACE_INET_Addr &local_addr);
 
  virtual ACE_HANDLE get_handle (void) const;
 
  virtual int handle_input (ACE_HANDLE handle);
  
  int mkpregled(void);

private:
  ACE_Process_Options proc_opt;
  ACE_Process proc;
};



// ---------------------------------------------

// Our Reactor Singleton.
typedef ACE_Singleton<ACE_Reactor, ACE_Null_Mutex>
	REACTOR;

// ---------------------------------------------

// Our ACE_Test_and_Set Singleton.
typedef ACE_Singleton<ACE_Test_and_Set <ACE_Null_Mutex, sig_atomic_t>, ACE_Null_Mutex>
	QUIT_HANDLER;


// ---------------------------------------------


Observer_Handler::Observer_Handler (const ACE_INET_Addr &local_addr)
  : ACE_SOCK_Dgram (local_addr, PF_INET, 0, 1)
{
  char optval = 1;

  ACE_SOCK_Dgram::set_option(SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
  // Nekoè bomo tole prebrali iz konfiguracije?
  proc_opt.command_line("/home/andrej/sola/volitve/src/admin/mk-pregled.py");
  //  proc_opt.setenv("PATH", "/home/andrej/sola/volitve/admin");
  proc_opt.working_directory("/home/andrej/sola/volitve/src/admin");

};

/*
int
Observer_Handler::handle_timeout (const ACE_Time_Value &,
				 const void *arg)
{
  ACE_ASSERT (arg == this);
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) handling timeout from this = %u\n", this));
  return 0;
}
*/

ACE_HANDLE
Observer_Handler::get_handle (void) const
{
  return ACE_SOCK_Dgram::get_handle ();
}

// Perform the market change receive.

int
Observer_Handler::handle_input (ACE_HANDLE)
{
  // Perform two recv's to emulate record-oriented semantics.  Note
  // that this code is not entirely portable since it relies on the
  // fact that sizeof (ssize_t) is the same on both the sender and
  // receiver side.  To correctly handle this is painful, and we leave
  // it as an exercise for the reader ;-).

  ACE_DEBUG((LM_DEBUG, "(%P|%t) Handle input \n"));

  ACE_INET_Addr from_addr;

  char buff[NOTIFIER_MESSAGE_LENGTH+1];

  ssize_t n = ACE_SOCK_Dgram::recv ((void *) &buff, NOTIFIER_MESSAGE_LENGTH, 
				    from_addr);
    
  buff[n] = '\0';

  switch (n)
    {
    case -1:
      ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p at host %s\n",
			"client logger", from_addr.get_host_name() ), 0);
      /* NOTREACHED */
    case 0:
      ACE_ERROR_RETURN ((LM_ERROR,
			 "(%P|%t) closing connection to host %s (fd = %d)\n",
			from_addr.get_host_name(), this->get_handle ()), 0);
      /* NOTREACHED */
    case NOTIFIER_MESSAGE_LENGTH:
      {	
	if (strlen (buff) == n)
	  {
	    ACE_DEBUG ((LM_DEBUG, "(%P|%t) Notification: length %d content '%s'\n", n, buff));
	    mkpregled();
	  }
	else
	  ACE_ERROR ((LM_ERROR, "(%P|%t) error, strlen(buff) = %d, n = %d\n",
		      strlen(buff), n));
	break;
      }
    default:
      ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p at host %s\n",
			"client", from_addr.get_host_name()), 0);
      /* NOTREACHED */
    }

  return 0;
}

int Observer_Handler::mkpregled(void) {
  if (proc.spawn(proc_opt) == -1) 
    ACE_ERROR_RETURN((LM_ERROR, "Process spawn returned -1\n"), -1);
  proc.wait();
}

// ---------------------------------------------

int 
main (int argc, char *argv[])
{  

  Observer_Handler observer(ACE_INET_Addr(NOTIFIER_DEFAULT_PORT));

  observer.mkpregled();

  if (REACTOR::instance ()->register_handler
      (SIGINT, QUIT_HANDLER::instance()) == -1)
   ACE_ERROR_RETURN ((LM_ERROR,
		      "registering service with ACE_Reactor\n"), -1);
  else if (REACTOR::instance ()->register_handler
	   (&observer, ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
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


