/* -*- C++ -*- */
/* 
 * $ProjectId$
 *
 * $Id: Notifier.h 1.1 Thu, 28 Aug 1997 17:15:39 +0000 andrej $
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

  int open (const ACE_INET_Addr &local_addr, const u_short BroadcastPort);
 
  virtual ACE_HANDLE get_handle (void) const;
 
  virtual int handle_output (ACE_HANDLE handle);
 
  /*  virtual int handle_timeout (const ACE_Time_Value & tv, 
                              const void *arg = 0);*/
private:
  u_short BroadcastPort_;
  int cnt;
};

typedef ACE_Singleton<Notifier, ACE_Null_Mutex>
        NOTIFIER;

#endif /* NOTIFIER_H */
