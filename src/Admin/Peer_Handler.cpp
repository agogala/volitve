/*
 * $ProjectHeader: volitve 0.26 Sat, 08 Nov 1997 08:02:11 +0100 andrej $
 *
 * $Id: Peer_Handler.cpp 1.4 Fri, 26 Sep 1997 16:28:00 +0000 andrej $
 *
 * Sprejema zahtevke od klientov.
 */

// Tokenizer:
#include <ace/Process.h>

#include "admind.h"
#include "Peer_Handler.h"
#include "Registrator.h"
#include "Utils.h"

Peer_Handler::Peer_Handler (void)
{
}

int
Peer_Handler::handle_timeout (const ACE_Time_Value &,
				 const void *arg)
{
  ACE_ASSERT (arg == this);
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) handling timeout from this = %u\n", this));
  return 0;
}

// Perform the logging record receive.

int
Peer_Handler::handle_input (ACE_HANDLE)
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
      { // case 1
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
	    // Prevedi zahtevke v klice registratorjevih procedur.
	    // Nobenega preverjanja napak, ¾al...
	    ACE_Tokenizer tokens(rs);
	    tokens.delimiter_replace(' ', '\0');
	    tokens.preserve_designators('"','"');
	    char *cmd = tokens.next();

	    ACE_DEBUG((LM_DEBUG, "CMD: %s\n", cmd));

	    if (!ACE_OS::strcasecmp(cmd, "Validate")) {
	      char *hash = tokens.next();
	      int rc = REGISTRATOR::instance()-> Validate(hash);
	      int2code(rc, &rs[1]);
	    } 
	    else if (!ACE_OS::strcasecmp(cmd, "Register")) {
	      char *hash = tokens.next();
	      char *username = tokens.next();
	      char *passwd = tokens.next();

	      int rc = REGISTRATOR::instance()-> Register
		(hash, username, passwd);
	      int2code(rc, &rs[1]);
	    }
	    else if (!ACE_OS::strcasecmp(cmd, "UserID")) {
	      char *username = tokens.next();

	      int rc = REGISTRATOR::instance()-> UserID(username, &rs[5]);
	      int2code(rc, &rs[1]);
	      rs[4] = ' ';

	    }
	    else {
	      // report error:
	      int2code(200, &rs[1]);
	    }
	    rs[0] = strlen(&rs[1]);

	    this->peer ().send_n ((void *) rs, rs[0]+1, 0);
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
Peer_Handler::open (void *)
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
template class ACE_Acceptor<Peer_Handler, ACE_LSOCK_ACCEPTOR>;
template class ACE_Svc_Handler<ACE_LSOCK_STREAM, ACE_NULL_SYNCH>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Acceptor<Peer_Handler, ACE_LSOCK_ACCEPTOR>
#pragma instantiate ACE_Svc_Handler<ACE_LSOCK_STREAM, ACE_NULL_SYNCH>
#endif
