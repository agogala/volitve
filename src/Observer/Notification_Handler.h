/* -*- C++ -*-
 * $ProjectHeader: volitve 0.18 Sun, 05 Oct 1997 22:53:12 +0200 andrej $
 *
 * $Id: Notification_Handler.h 1.2 Fri, 26 Sep 1997 16:28:00 +0000 andrej $
 *
 * Zaznava spremembe na trgu.
 */

#if !defined(NOTIFICATION_HANDLER_H)
#define NOTIFICATION_HANDLER_H

#include <ace/Event_Handler.h>
#include <ace/SOCK_Dgram.h>
#include <ace/INET_Addr.h>

#include "State.h"
#include "StrSet.h"

class Notification_Handler : public ACE_Event_Handler, public ACE_SOCK_Dgram
{
  // = TITLE
  //     Receive broadcast messages from market.
  //
  // = DESCRIPTION
  //     This class waits for broadcast messages from market and sets 
  //     appropriate flags.
public:
  Notification_Handler (const ACE_INET_Addr &local_addr, State *state,
			strset *userset);
 
  virtual ACE_HANDLE get_handle (void) const;
 
  virtual int handle_input (ACE_HANDLE handle);

private:
  State *state_;
  strset *userset_;

};

#endif // NOTIFICATION_HANDLER_H
