/* -*- C++ -*- */
/*
 * $ProjectHeader: volitve 0.6 Fri, 05 Sep 1997 16:43:33 +0200 andrej $
 *
 * $Id: marketd.h 1.3 Thu, 04 Sep 1997 02:15:14 +0000 andrej $
 *
 * Blagovna borza deamon.
 *
 */

#if !defined (MARKETD_H)
#define MARKETD_H

#include <ace/Singleton.h>
#include <ace/Reactor.h>
#include <ace/Synch.h>

// Our Reactor Singleton.
typedef ACE_Singleton<ACE_Reactor, ACE_Null_Mutex>
	REACTOR;

// Our ACE_Test_and_Set Singleton.
typedef ACE_Singleton<ACE_Test_and_Set <ACE_Null_Mutex, sig_atomic_t>, ACE_Null_Mutex>
	QUIT_HANDLER;

#endif // MARKETD_H
