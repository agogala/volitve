/*
 * $ProjectHeader: volitve 0.24 Mon, 03 Nov 1997 14:25:50 +0100 andrej $
 *
 * $Id: manager.cpp 1.3 Tue, 28 Oct 1997 20:15:29 +0000 andrej $
 *
 * Poganja in nadzira procese. Èe se kak sesuje, ga sku¹a ponovno
 * pognati. O sesutju tudi poroèa.
 */

#include <ace/OS.h>
#include <ace/Process.h>
#include <ace/Log_Msg.h>
#include <ace/Singleton.h>
#include <ace/Reactor.h>
#include <ace/Synch.h>

#include "Config.h"

// Our Reactor Singleton.
typedef ACE_Singleton<ACE_Reactor, ACE_Null_Mutex>
	REACTOR;

// Our ACE_Test_and_Set Singleton.
typedef ACE_Singleton<ACE_Test_and_Set <ACE_Null_Mutex, sig_atomic_t>, ACE_Null_Mutex>
	QUIT_HANDLER;

class Manager : public ACE_Event_Handler
{
public:
  Manager();
  ~Manager();

  virtual int open();
  virtual int close();

protected:
  virtual int handle_signal (
    int signum,
    siginfo_t * = 0,
    ucontext_t * = 0
    );
  
private:
  ACE_Process *observerd, *marketd, *admind;

  int run_observerd();
  int run_marketd();
  int run_admind();
};

int main()
{

  // Signals we react on:
  ACE_Sig_Set signals;
  signals.sig_add(SIGINT);
  signals.sig_add(SIGTERM);
  signals.sig_add(SIGHUP);
  
  // Register quit handler:
  if (REACTOR::instance ()->register_handler
	   (signals, QUIT_HANDLER::instance()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
		       "registering service with ACE_Reactor\n"), -1);
  
  // Manager instance:
  Manager manager;
    
  // Register with Reactor:
  if (REACTOR::instance ()->register_handler
      (SIGCHLD, &manager) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
		       "registering service with ACE_Reactor\n"), -1);
    
  // Run forever, managing children.
  ACE_DEBUG ((LM_DEBUG,
	      "(%P|%t) starting up manager:\n"));

  // Run processes:
  manager.open();

  // Perform logging service until QUIT_HANDLER receives SIGINT.
  while (QUIT_HANDLER::instance ()->is_set () == 0)
    REACTOR::instance ()->handle_events ();
  
  ACE_DEBUG ((LM_DEBUG,
	      "(%P|%t) shutting down manager\n"));

  manager.close();
  
  return 0;
}

Manager::Manager(void)
{
  observerd = NULL;
  marketd = NULL;
  admind = NULL;
}

// Pobije procese. Mogoèe posebna funkcija close()?
Manager::~Manager(void)
{
  close();
}

int 
Manager::open()
{
  // run processes:
  run_observerd();
  run_marketd();
  run_admind();

  return 0;
}

int Manager::close()
{
  // Remove ourselfs from REACTOR:
  REACTOR:: instance() ->remove_handler(SIGCHLD);

  if (admind!=NULL) {
    admind->kill();
    delete admind;
    admind = NULL;
  }
  
  if (marketd!=NULL) {
    marketd->kill();
    delete marketd;
    marketd = NULL;
  }

  if (observerd!=NULL) {
    observerd->kill();
    delete observerd;
    observerd = NULL;
  } 
  return 0;
}

int 
Manager::handle_signal (
    int signum,
    siginfo_t * = 0,
    ucontext_t * = 0
    )
{
  if (signum==SIGCHLD) {    
    ACE_DEBUG((LM_DEBUG, "Child killed!\n"));
    if (QUIT_HANDLER::instance ()->is_set () != 0) {
      return -1;
    }

    pid_t pid;
    int status;

    while ((pid = ACE_OS::waitpid (-1, &status, WNOHANG)) > 0) {

      if ((observerd!=NULL) && (pid==observerd->getpid())) {
	ACE_DEBUG ((LM_DEBUG, "Observerd exited\n"));
	run_observerd();
	continue;
      }

      if ((marketd!=NULL) && (pid==marketd->getpid())) {
	ACE_DEBUG((LM_DEBUG, "Marketd exited\n"));
	run_marketd();
	continue;
      }

      if ((admind!=NULL) && (pid==admind->getpid())) {
	ACE_DEBUG((LM_DEBUG, "Admind exited\n"));
	run_admind();
	continue;
      }

    }
    return 0;
  } else
    ACE_ERROR_RETURN((LM_ERROR, "Unknown signal: %d\n", signum), -1);
}

int Manager::run_observerd()
{
  ACE_Process_Options options;
  options.command_line ("./observerd");

  if (observerd==NULL)    
    observerd = new ACE_Process;
  // Zbri¹i socket...
  // Verjetno bi morali ¹e preveriti, èe proces obstaja....
  ACE_OS::unlink(FORMATER_DEFAULT_PATH);

  if (observerd->spawn (options) == -1)
    {
      int error = ACE_OS::last_error ();
      ACE_ERROR ((LM_ERROR, "%p errno = %d.\n",
                  "test_date", error));
      return error;
    }

  return 0;
}

int Manager::run_marketd()
{
  ACE_Process_Options options;
  options.command_line ("./marketd");

  if (marketd==NULL)    
    marketd = new ACE_Process;
  // Zbri¹i socket...
  // Verjetno bi morali ¹e preveriti, èe proces obstaja....
  ACE_OS::unlink(MARKET_DEFAULT_PATH);

  if (marketd->spawn (options) == -1)
    {
      int error = ACE_OS::last_error ();
      ACE_ERROR ((LM_ERROR, "%p errno = %d.\n",
                  "test_date", error));
      return error;
    }

  return 0;
}

int Manager::run_admind()
{
  ACE_Process_Options options;
  options.command_line ("./admind");

  if (admind==NULL)    
   admind = new ACE_Process;
  // Zbri¹i socket...
  // Verjetno bi morali ¹e preveriti, èe proces obstaja....
  ACE_OS::unlink(ADMIN_DEFAULT_PATH);

  if (admind->spawn (options) == -1)
    {
      int error = ACE_OS::last_error ();
      ACE_ERROR ((LM_ERROR, "%p errno = %d.\n",
                  "test_date", error));
      return error;
    }

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

