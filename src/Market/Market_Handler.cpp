// Default constructor.

#include "market.h"
#include "Request.h"
#include "Notifier.h"

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
	char rs[256]; /* Request string */

	// To je odveè, saj je len samo en byte!
	//        len = ntohl (len);
	n = this->peer ().recv_n ((void *) &rs, len);

	rs[n] = '\0';

	if (n != len)
	  ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p at host %s\n",
			    "client", this->peer_name_), -1);
	/* NOTREACHED */

	if (strlen (rs) == n)
	  {
	    ACE_DEBUG ((LM_DEBUG, "(%P|%t) Request: length %d content '%s'\n", n, rs));
	    int rc = REQUEST::instance ()-> Add(rs);

	    /* report back */
	    strcpy(&rs[1], REQUEST::instance ()-> Result());
	    rs[0] = strlen(&rs[1]);

	    this->peer ().send_n ((void *) rs, rs[0]+1, 0);

	    if (rc == 0) {
	      /* notify observers */
	      NOTIFIER::instance ()-> notify();
	    } 
	  }
	else
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
		"(%P|%t) connected to client %s\n", this->peer_name_));
      return 0;
}
