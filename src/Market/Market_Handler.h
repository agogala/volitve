/* -*- C++ -*- */
/*
 * $ProjectHeader: volitve 0.21 Tue, 21 Oct 1997 21:26:41 +0200 andrej $
 *
 * $Id: Market_Handler.h 1.2 Thu, 04 Sep 1997 02:15:14 +0000 andrej $
 *
 * Blagovna borza. Market_Handler sprejema in dela z odjemalci.
 *
 */

#if !defined (MARKET_HANDLER)
#define MARKET_HANDLER

#include <ace/Acceptor.h>
#include <ace/LSOCK_Acceptor.h>
#include <ace/Synch.h>

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

// Specialize a Market Acceptor.
typedef ACE_Acceptor <Market_Handler, ACE_LSOCK_ACCEPTOR>
	Market_Acceptor;

#endif /* MARKET_HANDLER */
