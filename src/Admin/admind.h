/* -*- C++ -*-
 * $ProjectHeader: volitve 0.20 Sun, 19 Oct 1997 19:07:54 +0200 andrej $
 *
 * $Id: admind.h 1.1 Thu, 11 Sep 1997 16:28:32 +0000 andrej $
 *
 * Observer deamon.
 */

#if !defined(OBSERVERD_H)
#define OBSERVERD_H

#include <ace/Reactor.h>
#include <ace/Synch.h>
#include <ace/Singleton.h>

// ---------------------------------------------

// Our Reactor Singleton.
typedef ACE_Singleton<ACE_Reactor, ACE_Null_Mutex>
	REACTOR;

// ---------------------------------------------

// Our ACE_Test_and_Set Singleton.
typedef ACE_Singleton<ACE_Test_and_Set <ACE_Null_Mutex, sig_atomic_t>, ACE_Null_Mutex>
	QUIT_HANDLER;

// ---------------------------------------------


#endif // OBSERVERD_H
