/* -*- C++ -*- */
/*
 * $ProjectId$
 *
 * $Id: market.h 1.1 Thu, 28 Aug 1997 17:15:39 +0000 andrej $
 *
 * Blagovna borza. Glavni program.
 *
 */

#if !defined (MARKET_H)
#define MARKET_H

#include <ace/Get_Opt.h>
#include <ace/Acceptor.h>
#include <ace/LSOCK_Acceptor.h>
#include <ace/Synch.h>
#include <ace/Singleton.h>

#include "Config.h"

#if defined (ACE_HAS_MINIMUM_IOSTREAMH_INCLUSION)
# include <iostream.h>
#endif /* ACE_HAS_MINIMUM_IOSTREAMH_INCLUSION */

// ----------------------------------------

class Market_Handler : public ACE_Svc_Handler<ACE_LSOCK_STREAM, ACE_NULL_SYNCH>
  // = TITLE
  //     Receive client message from the remote clients.
  //
  // = DESCRIPTION
  //     This class demonstrates how to receive messages from remote
  //     clients using the notification mechanisms in the
  //     <ACE_Reactor>.  In addition, it also illustrates how to
  //     utilize the <ACE_Reactor> timer mechanisms, as well.
{
public:
  // = Initialization and termination methods.
  Market_Handler (void);

  // = Hooks for opening and closing handlers.
  virtual int open (void *);

protected:
  // = Demultiplexing hooks.
  virtual int handle_input (ACE_HANDLE);
  virtual int handle_timeout (const ACE_Time_Value &tv,
			      const void *arg);

private:
  char peer_name_[MAXHOSTNAMELEN + 1];
  // Host we are connected to.
};

// Our Reactor Singleton.
typedef ACE_Singleton<ACE_Reactor, ACE_Null_Mutex>
	REACTOR;

// Our ACE_Test_and_Set Singleton.
typedef ACE_Singleton<ACE_Test_and_Set <ACE_Null_Mutex, sig_atomic_t>, ACE_Null_Mutex>
	QUIT_HANDLER;

// Specialize a Market Acceptor.
typedef ACE_Acceptor <Market_Handler, ACE_LSOCK_ACCEPTOR>
	Market_Acceptor;

#endif /* MARKET_H */
