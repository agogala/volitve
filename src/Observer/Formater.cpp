/* -*- C++ -*-
 * $ProjectHeader: volitve 0.28 Sat, 27 Dec 1997 16:06:49 +0100 andrej $
 *
 * $Id: Formater.cpp 1.1.2.6 Sat, 08 Nov 1997 07:02:11 +0000 andrej $
 *
 * Oblikuje HTML datoteke.
 */

#include <stdio.h>
#include <Python.h>

#include <ace/Process.h>

#include "Config.h"
#include "Formater.h"
#include "observerd.h"

Formater::Formater(State *state, strset *userset)
{
  this->state_ = state;
  this->userset_ = userset;
 
  this->initialized_ = false;
}

int Formater::init()
{
  if (initialized_)
    return 0;

  ACE_DEBUG((LM_DEBUG, "Initializing python interpreter\n"));

  // Pripravi interpeter
  Py_Initialize();

  // Nalo¾i skripte:
  FILE *f;
  do {

    if ((f = fopen("./initfmt.py", "r"))==NULL) {
      ACE_ERROR((LM_ERROR, "Can't find init script\n"));
      break;
    }


    if (PyRun_SimpleFile(f, "Formater")!=0) {
      ACE_ERROR((LM_ERROR, "Error initializing formater\n"));
      break;
    }

    fclose(f);

    initialized_ = true;

  } while (0);

  return initialized_ ? 0 : -1;
}

int Formater::handle_timeout(const ACE_Time_Value &,
                                 const void *arg)
{
  if (state_->is_set()) {
    mkPregled();
  }
  while (userset_->size()>0) {
    mkUser(*(userset_->begin()));
  }

  return 0;
}


int 
Formater::mkPregled()
{
  int rc = 0;

  if (!initialized_)
    init();

  ACE_DEBUG((LM_DEBUG, "Formatting pregled.\n"));

  if (PyRun_SimpleString("MakePregled.run(admin_cfg.tempdir, admin_cfg.htmldir, admin_cfg.templates)")) {
    ACE_ERROR((LM_ERROR, "Error running MakePregled\n"));
    rc = 100;
  }

  state_->set(false);
  return rc;
}

int
Formater::mkUser(const char *User)
{
  int rc=0;

  if (!initialized_)
    init();

  char cmd[500];

  ACE_OS::sprintf(cmd, "MakePregled.defupdateuser('%s')", User);
  ACE_DEBUG((LM_DEBUG, "Formatting: %s\n", User));
  if (PyRun_SimpleString(cmd)) {
    ACE_ERROR((LM_ERROR, "Error running MakePregled.defupdateuser\n"));
    rc = 100;
  }

  // Zbri¹i username, èe obstaja v mno¾ici.
  strset::iterator i = userset_->find(User);
  if (i!=userset_->end()) {
    const char *ime = *i;
    userset_->erase(i);
    delete ime;
  }

  return rc;
}

int
Formater::mkContracts(const char *User, const char *Paper)
{
  int rc=0;

  if (!initialized_)
    init();

  char cmd[500];

  ACE_OS::sprintf(cmd, "MakePogodbe.defaultrun('%s','%s')", User, Paper);
  ACE_DEBUG((LM_DEBUG, "Formatting contract: %s %s\n", User, Paper));
  if (PyRun_SimpleString(cmd)) {
    ACE_ERROR((LM_ERROR, "Error running MakePogodbe.defaultrun\n"));
    rc = 100;
  }

  return rc;
}

// Handler:

int
Formater_Handler::handle_input (ACE_HANDLE)
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
			"client logger", this->get_handle ()), -1);
      /* NOTREACHED */
    case 0:
      ACE_ERROR_RETURN ((LM_ERROR,
			 "(%P|%t) closing connection to (fd = %d)\n",
			this->get_handle ()), -1);
      /* NOTREACHED */
    case 1:
      { // case 1
	char rs[256]; /* Request string */

	// To je odveè, saj je len samo en byte!
	//        len = ntohl (len);
	n = this->peer ().recv_n ((void *) &rs, len);

	rs[n] = '\0';

	if (n != len)
	  ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p at fd = %d\n",
			    "client", this->get_handle (), this), -1);
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

	    if (!ACE_OS::strcasecmp(cmd, "MakePregled")) {
	      int rc=formater->mkPregled();
	      int2code(rc, &rs[1]);
	    } 
	    else if (!ACE_OS::strcasecmp(cmd, "MakeUser")) {
	      char *username = tokens.next();	      
	      int rc=formater->mkUser(username);
	      int2code(rc, &rs[1]);
	    }
	    else if (!ACE_OS::strcasecmp(cmd, "Contracts")) {
	      char *username = tokens.next();
	      char *paper = tokens.next();
	      int rc=formater->mkContracts(username, paper);
	      int2code(rc, &rs[1]);
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
      ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p at fd = %d\n",
			 "client", this->get_handle()), -1);
      /* NOTREACHED */
    }
  return 0;
}

int 
Formater_Handler::open(void *)
{
  if (REACTOR::instance ()->register_handler
      (this, ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
		       "(%P|%t) can't register with reactor\n"), -1);
  else
    ACE_DEBUG ((LM_DEBUG,
		"(%P|%t) connected to client (fd = %d)\n", 
		this->get_handle() ));
      return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Acceptor<Formater_Handler, ACE_LSOCK_ACCEPTOR>;
template class ACE_Svc_Handler<ACE_LSOCK_STREAM, ACE_NULL_SYNCH>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Acceptor<Formater_Handler, ACE_LSOCK_ACCEPTOR>
#pragma instantiate ACE_Svc_Handler<ACE_LSOCK_STREAM, ACE_NULL_SYNCH>
#endif
