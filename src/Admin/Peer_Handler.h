/* -*- C++ -*- */
/*
 * $ProjectHeader: volitve 0.13 Wed, 24 Sep 1997 19:03:46 +0200 andrej $
 *
 * $Id: Peer_Handler.h 1.1 Thu, 11 Sep 1997 16:28:32 +0000 andrej $
 *
 * Sprejema odjemalèeve zahtevke in klièe ustrezne registratorjeve funkcije.
 *
 */

#if !defined (PEER_HANDLER_H)
#define PEER_HANDLER_H

#include <ace/Acceptor.h>
#include <ace/LSOCK_Acceptor.h>
#include <ace/Synch.h>

#include "Config.h"

#if defined (ACE_HAS_MINIMUM_IOSTREAMH_INCLUSION)
# include <iostream.h>
#endif /* ACE_HAS_MINIMUM_IOSTREAMH_INCLUSION */

// ----------------------------------------

class Peer_Handler : public ACE_Svc_Handler<ACE_LSOCK_STREAM, ACE_NULL_SYNCH>
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
  Peer_Handler (void);

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
typedef ACE_Acceptor <Peer_Handler, ACE_LSOCK_ACCEPTOR>
	Peer_Acceptor;

#endif /* PEER_HANDLER */
