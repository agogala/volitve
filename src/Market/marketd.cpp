/*
 * $ProjectHeader: volitve 0.20 Sun, 19 Oct 1997 19:07:54 +0200 andrej $
 *
 * $Id: marketd.cpp 1.8 Wed, 24 Sep 1997 17:03:46 +0000 andrej $
 *
 * Glavni program.
 */

#include <ace/INET_Addr.h>
#include <ace/Get_Opt.h>
#include <ace/Object_Manager.h>
#include <ace/Signal.h>

#include "marketd.h"
#include "Notifier.h"
#include "Market.h"
#include "Market_Handler.h"

class Options
{
  // = TITLE
  //     Keeps track of the options.
public:
  void parse_args (int argc, char *argv[]);
  char *market_path (void);
  //  char *notifier_path (void);
  u_short broadcast_port (void);

private:
  char* market_path_;
  char *notifier_path_;
  u_short broadcast_port_;
};

// Our Options Singleton.
typedef ACE_Singleton<Options, ACE_Null_Mutex>
	OPTIONS;

// Funkcija, ki vse naredi. Posebej je zato, da lahko na koncu zbri¹emo 
// socket. (zaenkrat ne znam drugaèe...)
int ReactorLoop();

void CleanSocket(void *object, void *param);

int
main (int argc, char *argv[])
{

  OPTIONS::instance ()->parse_args (argc, argv);
  
  // Tole je prav za ACE 4.3. 
  // V kasnej¹ih verzijah je tole menda spremenjeno.
  // V resnici ne dela za vse signale. Pa bi menda moralo!
  ACE_Object_Manager::at_exit(OPTIONS::instance ()->market_path(),
		     CleanSocket, NULL);

  int rc = ReactorLoop();

  return rc;
}

void CleanSocket(void *object, void *param)
{
  // Zbri¹i socket.
  ACE_OS::unlink((char *)object);

}


// Handles all work:
int ReactorLoop()
{
  // Acceptor factory.
  Market_Acceptor peer_acceptor;

  //  Notifier notifier(ACE_INET_Addr (OPTIONS::instance ()->broadcast_port()),
  //  		    OPTIONS::instance ()->broadcast_port());
		    
  // We need to pass in REACTOR::instance () here so that we don't use
  // the default ACE_Reactor::instance ().

  ACE_Sig_Set signals;
  /*  signals.fill_set();
  signals.sig_del(SIGKILL);
  signals.sig_del(SIGSTOP);
  */
  signals.sig_add(SIGINT);
  signals.sig_add(SIGTERM);
  signals.sig_add(SIGHUP);
  
  // Pove¾i se z bazo [...] Bolj opisna napaka?
  if (MARKET::instance ()->open()==-1)
    ACE_ERROR_RETURN ((LM_ERROR, 
		       "Error opening database connection\n"), -1);

  if (peer_acceptor.open
      (ACE_UNIX_Addr (OPTIONS::instance ()->market_path ()),
       REACTOR::instance ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open"), -1);

  else if (REACTOR::instance ()->register_handler
	   (&peer_acceptor, ACE_Event_Handler::ACCEPT_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
		       "registering service with ACE_Reactor\n"), -1);

  // Register QUIT_HANDLER to receive SIGINT commands.  When received,
  // QUIT_HANDLER becomes "set" and thus, the event loop below will
  // exit.
  else if (REACTOR::instance ()->register_handler
	   (signals, QUIT_HANDLER::instance()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
		       "registering service with ACE_Reactor\n"), -1);
  
  // Open Notifier:
  /*  else if (NOTIFIER::instance ()->open
      //	   (ACE_INET_Addr(50000), 
      //	    NOTIFIER_DEFAULT_PORT) == -1)
      (ACE_INET_Addr(NOTIFIER_DEFAULT_PORT),
       ACE_INET_Addr(NOTIFIER_DEFAULT_PORT)) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open"), -1);
  // Register Notifier
  else if (REACTOR::instance ()->register_handler
	   (NOTIFIER::instance (), ACE_Event_Handler::WRITE_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
    "registering service with ACE_Reactor\n"), -1);

  // Disable Notifier
  REACTOR::instance ()->suspend_handler( NOTIFIER::instance () );
  */
  
  // Notifierja klièemo kar direktno, ne bomo ga klicali prek reactorja
  else if (NOTIFIER::instance ()->open
	   (ACE_INET_Addr(NOTIFIER_LOCAL_PORT),
	    ACE_INET_Addr(NOTIFIER_DEFAULT_PORT)) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open notifier"), -1);

  // Run forever, performing market service.

  ACE_DEBUG ((LM_DEBUG,
	      "(%P|%t) starting up market daemon\n"));

  // Perform logging service until QUIT_HANDLER receives SIGINT.
  while (QUIT_HANDLER::instance ()->is_set () == 0)
    REACTOR::instance ()->handle_events ();

  ACE_DEBUG ((LM_DEBUG,
	      "(%P|%t) shutting down market daemon\n"));

  return 0;
}

// Options implementation:

char 
*Options::market_path (void)
{
  return this->market_path_;
}

/*char
*Options::notifier_path (void)
{
  return this->notifier_path_;
}*/

u_short
Options::broadcast_port (void)
{
  return this->broadcast_port_;
}

// Parse the command-line options.

void
Options::parse_args (int argc, char *argv[])
{
  this->market_path_ = MARKET_DEFAULT_PATH;
  //  this->notifier_path_ = NOTIFIER_DEFAULT_PATH;
  this->broadcast_port_ = NOTIFIER_DEFAULT_PORT;

  ACE_Get_Opt get_opt (argc, argv, "m:b:");

  for (int c; (c = get_opt ()) != -1; )
     switch (c)
       {
       case 'm':
	 this->market_path_ = ACE_OS::strdup (get_opt.optarg);
	 break;
       case 'b':
	 this->broadcast_port_ = ACE_OS::atoi (get_opt.optarg);
	 break;
       default:
	 break;
       }
}

// Instantiations:

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton<ACE_Reactor, ACE_Null_Mutex>;
template class ACE_Singleton<ACE_Test_and_Set <ACE_Null_Mutex, sig_atomic_t>, ACE_Null_Mutex>;
template class ACE_Singleton<Options, ACE_Null_Mutex>;
template class ACE_Svc_Handler<ACE_LSOCK_STREAM, ACE_NULL_SYNCH>;
template class ACE_Test_and_Set<ACE_Null_Mutex, sig_atomic_t>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton<ACE_Reactor, ACE_Null_Mutex>
#pragma instantiate ACE_Singleton<ACE_Test_and_Set <ACE_Null_Mutex, sig_atomic_t>, ACE_Null_Mutex>
#pragma instantiate ACE_Singleton<Options, ACE_Null_Mutex>
#pragma instantiate ACE_Svc_Handler<ACE_LSOCK_STREAM, ACE_NULL_SYNCH>
#pragma instantiate ACE_Test_and_Set<ACE_Null_Mutex, sig_atomic_t>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

