/*
 * $ProjectHeader: volitve 0.15 Fri, 26 Sep 1997 18:28:00 +0200 andrej $
 *
 * $Id: Market_Handler.cpp 1.8 Fri, 26 Sep 1997 16:28:00 +0000 andrej $
 *
 * Sprejema zahtevke od klientov.
 */

#include "marketd.h"
#include "Market.h"
#include "Request.h"
#include "Market_Handler.h"
#include "Notifier.h"
#include "StrSet.h"
#include "Utils.h"

Market_Handler::Market_Handler (void)
{
}

int
Market_Handler::handle_timeout (const ACE_Time_Value &,
				 const void *arg)
{
  ACE_ASSERT (arg == this);
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) handling timeout from this = %u\n", this));
  return 0;
}

// Perform the logging record receive.

int
Market_Handler::handle_input (ACE_HANDLE)
{
  // Perform two recv's to emulate record-oriented semantics.  Note
  // that this code is not entirely portable since it relies on the
  // fact that sizeof (ssize_t) is the same on both the sender and
  // receiver side.  To correctly handle this is painful, and we leave
  // it as an exercise for the reader ;-).

  char len;
  ssize_t n = this->peer ().recv ((void *) &len, 1);

  switch (n)
    {
    case -1:
      ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p at host %s\n",
			"client logger", this->peer_name_), -1);
      /* NOTREACHED */
    case 0:
      ACE_ERROR_RETURN ((LM_ERROR,
			 "(%P|%t) closing connection to host %s (fd = %d)\n",
			this->peer_name_, this->get_handle ()), -1);
      /* NOTREACHED */
    case 1:
      {
	char rs[256]; // Request string 

	n = this->peer ().recv_n ((void *) &rs, len);

	rs[n] = '\0';

	if (n != len)
	  ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p at host %s\n",
			    "client", this->peer_name_), -1);
	/* NOTREACHED */

	if (strlen (rs) == n) {
	    ACE_DEBUG ((LM_DEBUG, "(%P|%t) Request: length %d content '%s'\n", n, rs));
	    Request req(rs);
	    strset userset;

	    int rc = MARKET::instance ()-> Add(req, &userset);

	    // report back 
	    int2code(rc, &rs[1]);
	    rs[0] = strlen(&rs[1]);

	    this->peer ().send_n ((void *) rs, rs[0]+1, 0);

	    if (rc == 0) {
	      // notify observers 
	      NOTIFIER::instance ()-> notify();
	      while (userset.size()>0) {
		NOTIFIER::instance ()-> notify(*userset.begin());
		const char *ime = *userset.begin();
		userset.erase(userset.begin());
		delete ime;
	      }
	    } else { // Poèistimo za seboj
	      while (userset.size()>0) {
		const char *ime = *userset.begin();
		userset.erase(userset.begin());
		delete ime;
	      }
	    }
	} else
	  ACE_ERROR ((LM_ERROR, "(%P|%t) error, strlen(rs) = %d, n = %d\n",
		      strlen(rs), n));
	break;
      }
    default:
      ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p at host %s\n",
			"client", this->peer_name_), -1);
      /* NOTREACHED */
    }
  return 0;
}

int
Market_Handler::open (void *)
{
  
  ACE_OS::strncpy(this->peer_name_, "LSOCK", MAXHOSTNAMELEN + 1);

  if (REACTOR::instance ()->register_handler
      (this, ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
		       "(%P|%t) can't register with reactor\n"), -1);
  /*
  else if (REACTOR::instance ()->schedule_timer
	       (this,
	       (const void *) this,
	       ACE_Time_Value (2),
	       ACE_Time_Value (2)) == -1)
	       ACE_ERROR_RETURN ((LM_ERROR,
	       "can'(%P|%t) t register with reactor\n"), -1);
  */
  else
    ACE_DEBUG ((LM_DEBUG,
		"(%P|%t) connected to client %s (fd = %d)\n", 
		this->peer_name_, this->get_handle() ));
      return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Acceptor<Market_Handler, ACE_LSOCK_ACCEPTOR>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Acceptor<Market_Handler, ACE_LSOCK_ACCEPTOR>
#endif
