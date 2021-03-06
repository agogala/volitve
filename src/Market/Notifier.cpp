/*
 * $ProjectHeader: volitve 0.28 Sat, 27 Dec 1997 16:06:49 +0100 andrej $
 *
 * $Id: Notifier.cpp 1.7 Tue, 04 Nov 1997 18:56:32 +0000 andrej $
 *
 * Po?lji broadcast, ?e se je zgodila kaka sprememba na trgu.
 */

#include "marketd.h"
#include "Notifier.h"
#include "Config.h"

Notifier::Notifier (void)
{
  //  this->BroadcastAddr_ = 0;
  this->cnt = 0;
  this->opened = false;
}

int Notifier::open
(const ACE_INET_Addr &local_addr, const ACE_INET_Addr BroadcastAddr)
{
   if (ACE_SOCK_Dgram_Bcast::open (local_addr)==-1)
     return -1;
   /*
   char optval = 1;

   ACE_SOCK_Dgram_Bcast::set_option(SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
   */
   this->BroadcastAddr_ = BroadcastAddr;

   this->opened = true;

   return 0;
}

ACE_HANDLE Notifier::get_handle (void) const
{
  return ACE_SOCK_Dgram_Bcast::get_handle();
}
 
/*int Notifier::handle_output (ACE_HANDLE handle)
{
  char msg[NOTIFIER_MESSAGE_SIZE];

  ACE_OS::sprintf(msg, "%08d", this->cnt);

  ACE_DEBUG ((LM_DEBUG,
		"(%P|%t) Handle output: %08d\n", this->cnt));
  ACE_SOCK_Dgram_Bcast::send(msg, ACE_OS::strlen(msg), this->BroadcastAddr_);
  REACTOR::instance ()->suspend_handler( this );

  this->cnt++;

  return 0;
}
*/
// Tole lahko kaj hitro spremenimo v funkcijo, ki nastavi flag, 
// vsake toliko ?asa pa pregledamo flag in obvestimo doti?ne...
int Notifier::notify ()
{
  if (!opened)
    ACE_ERROR_RETURN((LM_ERROR, "Notifier not opened\n"), -1);
  
  char msg[NOTIFIER_MESSAGE_LENGTH+1];

  ACE_OS::sprintf(msg, "MarketChange %0*d", NOTIFIER_MESSAGE_LENGTH-13, this->cnt);

  ACE_DEBUG ((LM_DEBUG,
		"(%P|%t) Handle output: %s\n", msg));
  /*  ACE_DEBUG ((LM_DEBUG,
	      "Broadcast address: %s, %d\n", 
	      this->BroadcastAddr_.get_host_addr(),
	      this->BroadcastAddr_.get_port_number()));

  ACE_DEBUG((LM_DEBUG,
	     "msg len: %d\n", ACE_OS::strlen(msg)));
	     */

  ACE_SOCK_Dgram_Bcast::send(msg, ACE_OS::strlen(msg), this->BroadcastAddr_);

  this->cnt++;

  return 0;
}

int Notifier::notify (const char *user)
{
  if (!opened)
    ACE_ERROR_RETURN((LM_ERROR, "Notifier not opened\n"), -1);
  
  char msg[NOTIFIER_MESSAGE_LENGTH+1] = "Change \"";

  strncpy(msg + 8, user, NOTIFIER_MESSAGE_LENGTH - 8);

  strcpy(msg + strlen(msg), "\"");

  ACE_DEBUG ((LM_DEBUG,
		"(%P|%t) Handle output: %s\n", msg));

  ACE_SOCK_Dgram_Bcast::send(msg, ACE_OS::strlen(msg), this->BroadcastAddr_);

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton<Notifier, ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton<Notifier, ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
