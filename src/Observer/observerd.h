/* -*- C++ -*-
 * $ProjectHeader: volitve 0.24 Mon, 03 Nov 1997 14:25:50 +0100 andrej $
 *
 * $Id: observerd.h 1.2 Tue, 28 Oct 1997 20:15:29 +0000 andrej $
 *
 * Observer deamon.
 */

#if !defined(OBSERVERD_H)
#define OBSERVERD_H

#include <ace/Reactor.h>
#include <ace/Synch.h>
#include <ace/Singleton.h>

#include "Formater.h"

// ---------------------------------------------

// Our Reactor Singleton.
typedef ACE_Singleton<ACE_Reactor, ACE_Null_Mutex>
	REACTOR;

// ---------------------------------------------

// Our ACE_Test_and_Set Singleton.
typedef ACE_Singleton<ACE_Test_and_Set <ACE_Null_Mutex, sig_atomic_t>, ACE_Null_Mutex>
	QUIT_HANDLER;

// ---------------------------------------------
extern Formater *formater;



#endif // OBSERVERD_H
