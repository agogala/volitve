/*
 * $ProjectId$
 *
 * $Id: Notifier.cpp 1.1 Thu, 28 Aug 1997 17:15:39 +0000 andrej $
 *
 * Po¹lji broadcast, èe se je zgodila kaka sprememba na trgu.
 */

#include "market.h"
#include "Notifier.h"

Notifier::Notifier (void)
{
  this->BroadcastPort_ = 0;
  this->cnt = 0;
}

int Notifier::open
(const ACE_INET_Addr &local_addr, const u_short BroadcastPort)
{
   if (ACE_SOCK_Dgram_Bcast::open (local_addr)==-1)
     return -1;
   char optval = 1;

   ACE_SOCK_Dgram_Bcast::set_option(SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
   this->BroadcastPort_ = BroadcastPort;
}

ACE_HANDLE Notifier::get_handle (void) const
{
  return ACE_SOCK_Dgram_Bcast::get_handle();
}
 
int Notifier::handle_output (ACE_HANDLE handle)
{
  char msg[NOTIFIER_MESSAGE_SIZE];

  ACE_OS::sprintf(msg, "%08d", this->cnt);

  ACE_DEBUG ((LM_DEBUG,
		"(%P|%t) Handle output: %08d\n", this->cnt));
  ACE_SOCK_Dgram_Bcast::send(msg, ACE_OS::strlen(msg), this->BroadcastPort_);
  REACTOR::instance ()->suspend_handler( NOTIFIER::instance () );

  this->cnt++;

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton<Notifier, ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton<Notifier, ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
