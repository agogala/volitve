/* -*- C++ -*-
 * $ProjectHeader: volitve 0.15 Fri, 26 Sep 1997 18:28:00 +0200 andrej $
 *
 * $Id: observerd.h 1.1 Mon, 08 Sep 1997 15:37:41 +0000 andrej $
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
