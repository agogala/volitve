/* -*- C++ -*-
 * $ProjectHeader: volitve 0.9 Wed, 10 Sep 1997 18:15:50 +0200 andrej $
 *
 * $Id: Notification_Handler.cpp 1.1.2.1 Wed, 10 Sep 1997 16:15:50 +0000 andrej $
 *
 * Zaznava spremembe na trgu.
 */

// ---------------------------------------------

#include <Notification_Handler.h>
#include <Formater.h>

#include "Config.h"

Notification_Handler::Notification_Handler (const ACE_INET_Addr &local_addr,
					    State *state)
  : ACE_SOCK_Dgram (local_addr, PF_INET, 0, 1)
{
  state_ = state;

  char optval = 1;

  ACE_SOCK_Dgram::set_option(SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
};

/*
int
Notification_Handler::handle_timeout (const ACE_Time_Value &,
				 const void *arg)
{
  ACE_ASSERT (arg == this);
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) handling timeout from this = %u\n", this));
  return 0;
}
*/

ACE_HANDLE
Notification_Handler::get_handle (void) const
{
  return ACE_SOCK_Dgram::get_handle ();
}

// Perform the market change receive.

int
Notification_Handler::handle_input (ACE_HANDLE)
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
	    state_->set(1);
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
