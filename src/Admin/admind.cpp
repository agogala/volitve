/*
 * $ProjectHeader: volitve 0.26 Sat, 08 Nov 1997 08:02:11 +0100 andrej $
 *
 * $Id: admind.cpp 1.2 Thu, 25 Sep 1997 19:32:05 +0000 andrej $
 *
 * Observer deamon. 
 */

#include <ace/Get_Opt.h>

#include "admind.h"
#include "Registrator.h"
#include "Peer_Handler.h"

#include "Config.h"

class Options
{
  // = TITLE
  //     Keeps track of the options.
public:
  void parse_args (int argc, char *argv[]);
  char *admin_path (void);

private:
  char *admin_path_;
};

// Our Options Singleton.
typedef ACE_Singleton<Options, ACE_Null_Mutex>
	OPTIONS;

int ReactorLoop();

int 
main (int argc, char *argv[])
{  
  OPTIONS::instance()-> parse_args(argc, argv);

  int rc = ReactorLoop();

  ACE_OS::unlink(OPTIONS::instance()-> admin_path());

  return rc;
}

int ReactorLoop()
{  

  ACE_Sig_Set signals;

  signals.sig_add(SIGINT);
  signals.sig_add(SIGTERM);
  signals.sig_add(SIGHUP);

  Peer_Acceptor peer_acceptor;

  if (peer_acceptor.open
      (ACE_UNIX_Addr (OPTIONS::instance()-> admin_path ()),
       REACTOR::instance ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open"), -1);
  else if (REACTOR::instance ()->register_handler
	   (&peer_acceptor, ACE_Event_Handler::ACCEPT_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
		       "can'(%P|%t) can't register with reactor\n"), -1);
  else if (REACTOR::instance ()->register_handler
	   (signals, QUIT_HANDLER::instance()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
		       "registering service with ACE_Reactor\n"), -1);

  ACE_DEBUG ((LM_DEBUG,
	      "(%P|%t) starting up administrator daemon\n"));
  
  // Perform administrator service until QUIT_HANDLER receives SIGINT.
  while (QUIT_HANDLER::instance () ->is_set () == 0)
    REACTOR::instance ()->handle_events ();
  
  ACE_DEBUG ((LM_DEBUG,
	      "(%P|%t) shutting down administrator daemon\n"));
  
  return 0;
}

// Options implementation:

char 
*Options::admin_path (void)
{
  return this->admin_path_;
}

// Parse the command-line options.

void
Options::parse_args (int argc, char *argv[])
{
  this->admin_path_ = ADMIN_DEFAULT_PATH;

  ACE_Get_Opt get_opt (argc, argv, "a");

  for (int c; (c = get_opt ()) != -1; )
     switch (c)
       {
       case 'a':
	 this->admin_path_ = ACE_OS::strdup (get_opt.optarg);
	 break;
       }
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton<ACE_Reactor, ACE_Null_Mutex>;
template class ACE_Singleton<ACE_Test_and_Set <ACE_Null_Mutex, sig_atomic_t>, ACE_Null_Mutex>;
template class ACE_Test_and_Set<ACE_Null_Mutex, sig_atomic_t>;
template class ACE_Singleton<Options, ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton<ACE_Reactor, ACE_Null_Mutex>
#pragma instantiate ACE_Singleton<ACE_Test_and_Set <ACE_Null_Mutex, sig_atomic_t>, ACE_Null_Mutex>
#pragma instantiate ACE_Test_and_Set<ACE_Null_Mutex, sig_atomic_t>
#pragma instantiate ACE_Singleton<Options, ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

