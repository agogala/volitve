/* -*- C++ -*- */
/* 
 * $ProjectHeader: volitve 0.24 Mon, 03 Nov 1997 14:25:50 +0100 andrej $
 *
 * $Id: Notifier.h 1.4 Thu, 25 Sep 1997 19:32:05 +0000 andrej $
 *
 * Zaznava spremembe na trgu in po¹ilja broadcast sporoèila o tem.
 */

#if !defined (NOTIFIER_H)
#define NOTIFIER_H

#include <ace/Synch.h>
#include <ace/Event_Handler.h>
#include <ace/SOCK_Dgram_Bcast.h>
#include <ace/Singleton.h>

#if defined (ACE_HAS_MINIMUM_IOSTREAMH_INCLUSION)
# include <iostream.h>
#endif /* ACE_HAS_MINIMUM_IOSTREAMH_INCLUSION */

// ----------------------------------------

class Notifier : public ACE_Event_Handler, public ACE_SOCK_Dgram_Bcast
{
public:
  Notifier (void);

  int open (const ACE_INET_Addr &local_addr, const ACE_INET_Addr BroadcastAddr);
 
  //  int open (const ACE_UNIX_Addr &local_addr, const ACE_UNIX_Addr BroadcastAddr);
 
  virtual ACE_HANDLE get_handle (void) const;
 
  //  virtual int handle_output (ACE_HANDLE handle);
  int notify();

  int notify(const char* user);
 
  /*  virtual int handle_timeout (const ACE_Time_Value & tv, 
                              const void *arg = 0);*/
private:
  ACE_INET_Addr BroadcastAddr_;
  int cnt;
  bool opened;
};

typedef ACE_Singleton<Notifier, ACE_Null_Mutex>
        NOTIFIER;

#endif /* NOTIFIER_H */
