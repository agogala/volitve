/* -*- C++ -*- */
/*
 * $ProjectHeader: volitve D.2 Wed, 03 Sep 1997 07:39:11 +0200 andrej $
 *
 * $Id: marketd.h 1.2 Wed, 03 Sep 1997 05:39:11 +0000 andrej $
 *
 * Blagovna borza deamon.
 *
 */

#ifndef (MARKETD_H)
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

#endif
